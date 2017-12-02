"""
Tests for TCP connection closing.
"""

from __future__ import print_function
from testers import functional
from helpers import analyzer, deproxy, chains
import asyncore

__author__ = 'Tempesta Technologies, Inc.'
__copyright__ = 'Copyright (C) 2017 Tempesta Technologies, Inc.'
__license__ = 'GPL2'


class CloseConnection(functional.FunctionalTest):
    """Regular connection closing."""

    def stop_and_close(self):
        '''To check the correctness of connection closing - we need to close
        it before stopping sniffer and analyzing sniffer's output (and throwing
        an exception in case of failure); so, we need to close Deproxy client
        and server connections in test_* function (not in tearDown).
        '''
        asyncore.close_all()
        self.client.close()
        self.client = None
        self.tempesta.stop()
        self.tempesta = None
        self.tester.close_all()
        self.tester = None

    def create_sniffer(self):
        self.sniffer = analyzer.AnalyzerCloseRegular(self.tempesta.node,
                                                     self.tempesta.host,
                                                     node_close=False,
                                                     timeout=10)

    def assert_results(self):
        self.assertTrue(self.sniffer.check_results(),
                        msg='Incorrect FIN-ACK sequence detected.')

    def create_chains(self):
        return [chains.base(forward=True)]

    def run_sniffer(self):
        self.sniffer.start()
        self.generic_test_routine('cache 0;\n', self.create_chains())
        self.stop_and_close()
        self.sniffer.stop()

    def test(self):
        self.create_sniffer()
        self.run_sniffer()
        self.assert_results()


class CloseConnectionError403(CloseConnection):
    """Connection closing due to 403 error, generated by Tempesta."""

    def assert_tempesta(self):
        pass

    def create_chains(self):
        chain_200 = chains.base(forward=True)
        chain_200.request.body = ''.join(['Arbitrary data ' for _ in range(300)])
        chain_200.request.update()
        response_403 = deproxy.Response.create(
            status=403,
            headers=['Content-Length: 0'],
            date=deproxy.HttpMessage.date_time_string()
        )
        chain_403 = deproxy.MessageChain(request = deproxy.Request(),
                                         expected_response = response_403)
        return [chain_200, chain_403]

    def create_sniffer(self):
        self.sniffer = analyzer.AnalyzerCloseRegular(self.tempesta.node,
                                                     self.tempesta.host,
                                                     timeout=10)

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
