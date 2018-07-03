/**
 *		Tempesta TLS
 *
 * The generic cipher wrapper.
 *
 * Adriaan de Jong <dejong@fox-it.com>
 *
 * Copyright (C) 2006-2018, Arm Limited (or its affiliates), All Rights Reserved
 * Copyright (C) 2015-2018 Tempesta Technologies, Inc.
 * SPDX-License-Identifier: GPL-2.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef TTLS_CIPHER_H
#define TTLS_CIPHER_H

#include "config.h"

#define TTLS_ERR_CIPHER_FEATURE_UNAVAILABLE  -0x6080  /**< The selected feature is not available. */
#define TTLS_ERR_CIPHER_BAD_INPUT_DATA	   -0x6100  /**< Bad input parameters. */
#define TTLS_ERR_CIPHER_ALLOC_FAILED		 -0x6180  /**< Failed to allocate memory. */
#define TTLS_ERR_CIPHER_INVALID_PADDING	  -0x6200  /**< Input data contains invalid padding and is rejected. */
#define TTLS_ERR_CIPHER_FULL_BLOCK_EXPECTED  -0x6280  /**< Decryption of block requires a full block. */
#define TTLS_ERR_CIPHER_AUTH_FAILED		  -0x6300  /**< Authentication failed (for AEAD modes). */
#define TTLS_ERR_CIPHER_INVALID_CONTEXT	  -0x6380  /**< The context is invalid. For example, because it was freed. */
#define TTLS_ERR_CIPHER_HW_ACCEL_FAILED	  -0x6400  /**< Cipher hardware accelerator failed. */

#define TTLS_CIPHER_VARIABLE_IV_LEN	 0x01	/**< Cipher accepts IVs of variable length. */
#define TTLS_CIPHER_VARIABLE_KEY_LEN	0x02	/**< Cipher accepts keys of variable length. */

/**
 * \brief	 An enumeration of supported ciphers.
 *
 * \warning   ARC4 and DES are considered weak ciphers and their use
 *			constitutes a security risk. We recommend considering stronger
 *			ciphers instead.
 */
typedef enum {
	TTLS_CIPHER_ID_NONE = 0,
	TTLS_CIPHER_ID_NULL,
	TTLS_CIPHER_ID_AES,
	TTLS_CIPHER_ID_DES,
	TTLS_CIPHER_ID_3DES,
	TTLS_CIPHER_ID_CAMELLIA,
	TTLS_CIPHER_ID_BLOWFISH,
	TTLS_CIPHER_ID_ARC4,
} ttls_cipher_id_t;

/**
 * \brief	 An enumeration of supported (cipher, mode) pairs.
 *
 * \warning   ARC4 and DES are considered weak ciphers and their use
 *			constitutes a security risk. We recommend considering stronger
 *			ciphers instead.
 */
typedef enum {
	TTLS_CIPHER_NONE = 0,
	TTLS_CIPHER_NULL,
	TTLS_CIPHER_AES_128_ECB,
	TTLS_CIPHER_AES_192_ECB,
	TTLS_CIPHER_AES_256_ECB,
	TTLS_CIPHER_AES_128_CBC,
	TTLS_CIPHER_AES_192_CBC,
	TTLS_CIPHER_AES_256_CBC,
	TTLS_CIPHER_AES_128_CFB128,
	TTLS_CIPHER_AES_192_CFB128,
	TTLS_CIPHER_AES_256_CFB128,
	TTLS_CIPHER_AES_128_CTR,
	TTLS_CIPHER_AES_192_CTR,
	TTLS_CIPHER_AES_256_CTR,
	TTLS_CIPHER_AES_128_GCM,
	TTLS_CIPHER_AES_192_GCM,
	TTLS_CIPHER_AES_256_GCM,
	TTLS_CIPHER_CAMELLIA_128_ECB,
	TTLS_CIPHER_CAMELLIA_192_ECB,
	TTLS_CIPHER_CAMELLIA_256_ECB,
	TTLS_CIPHER_CAMELLIA_128_CBC,
	TTLS_CIPHER_CAMELLIA_192_CBC,
	TTLS_CIPHER_CAMELLIA_256_CBC,
	TTLS_CIPHER_CAMELLIA_128_CFB128,
	TTLS_CIPHER_CAMELLIA_192_CFB128,
	TTLS_CIPHER_CAMELLIA_256_CFB128,
	TTLS_CIPHER_CAMELLIA_128_CTR,
	TTLS_CIPHER_CAMELLIA_192_CTR,
	TTLS_CIPHER_CAMELLIA_256_CTR,
	TTLS_CIPHER_CAMELLIA_128_GCM,
	TTLS_CIPHER_CAMELLIA_192_GCM,
	TTLS_CIPHER_CAMELLIA_256_GCM,
	TTLS_CIPHER_DES_ECB,
	TTLS_CIPHER_DES_CBC,
	TTLS_CIPHER_DES_EDE_ECB,
	TTLS_CIPHER_DES_EDE_CBC,
	TTLS_CIPHER_DES_EDE3_ECB,
	TTLS_CIPHER_DES_EDE3_CBC,
	TTLS_CIPHER_BLOWFISH_ECB,
	TTLS_CIPHER_BLOWFISH_CBC,
	TTLS_CIPHER_BLOWFISH_CFB64,
	TTLS_CIPHER_BLOWFISH_CTR,
	TTLS_CIPHER_ARC4_128,
	TTLS_CIPHER_AES_128_CCM,
	TTLS_CIPHER_AES_192_CCM,
	TTLS_CIPHER_AES_256_CCM,
	TTLS_CIPHER_CAMELLIA_128_CCM,
	TTLS_CIPHER_CAMELLIA_192_CCM,
	TTLS_CIPHER_CAMELLIA_256_CCM,
} ttls_cipher_type_t;

/** Supported cipher modes. */
typedef enum {
	TTLS_MODE_NONE = 0,
	TTLS_MODE_ECB,
	TTLS_MODE_CBC, /* Unsupported */
	TTLS_MODE_CFB, /* Unsupported */
	TTLS_MODE_OFB, /* Unused! */
	TTLS_MODE_CTR, /* Unsupported */
	TTLS_MODE_GCM,
	TTLS_MODE_STREAM, /* Unused! */
	TTLS_MODE_CCM,
} ttls_cipher_mode_t;

/** Supported cipher padding types. */
typedef enum {
	TTLS_PADDING_PKCS7 = 0,	 /**< PKCS7 padding (default).		*/
	TTLS_PADDING_ONE_AND_ZEROS, /**< ISO/IEC 7816-4 padding.		 */
	TTLS_PADDING_ZEROS_AND_LEN, /**< ANSI X.923 padding.			 */
	TTLS_PADDING_ZEROS,		 /**< zero padding (not reversible). */
	TTLS_PADDING_NONE,		  /**< never pad (full blocks only).   */
} ttls_cipher_padding_t;

/** Type of operation. */
typedef enum {
	TTLS_OPERATION_NONE = -1,
	TTLS_DECRYPT = 0,
	TTLS_ENCRYPT,
} ttls_operation_t;

enum {
	/** Undefined key length. */
	TTLS_KEY_LENGTH_NONE = 0,
	/** Key length, in bits (including parity), for DES keys. */
	TTLS_KEY_LENGTH_DES  = 64,
	/** Key length in bits, including parity, for DES in two-key EDE. */
	TTLS_KEY_LENGTH_DES_EDE = 128,
	/** Key length in bits, including parity, for DES in three-key EDE. */
	TTLS_KEY_LENGTH_DES_EDE3 = 192,
};

/** Maximum length of any IV, in Bytes. */
#define TTLS_MAX_IV_LENGTH	  16
/** Maximum block size of any cipher, in Bytes. */
#define TTLS_MAX_BLOCK_LENGTH   16

/**
 * Base cipher information (opaque struct).
 */
typedef struct ttls_cipher_base_t ttls_cipher_base_t;

/**
 * CMAC context (opaque struct).
 */
typedef struct ttls_cmac_context_t ttls_cmac_context_t;

/**
 * Cipher information. Allows calling cipher functions
 * in a generic way.
 */
typedef struct {
	/** Full cipher identifier. For example,
	 * TTLS_CIPHER_AES_256_CBC.
	 */
	ttls_cipher_type_t type;

	/** The cipher mode. For example, TTLS_MODE_CBC. */
	ttls_cipher_mode_t mode;

	/** The cipher key length, in bits. This is the
	 * default length for variable sized ciphers.
	 * Includes parity bits for ciphers like DES.
	 */
	unsigned int key_bitlen;

	/** Name of the cipher. */
	const char * name;

	/** IV or nonce size, in Bytes.
	 * For ciphers that accept variable IV sizes,
	 * this is the recommended size.
	 */
	unsigned int iv_size;

	/** Flags to set. For example, if the cipher supports variable IV sizes or variable key sizes. */
	int flags;

	/** The block size, in Bytes. */
	unsigned int block_size;

	/** Struct for base cipher information and functions. */
	const ttls_cipher_base_t *base;

} ttls_cipher_info_t;

/**
 * Generic cipher context.
 */
typedef struct {
	/** Information about the associated cipher. */
	const ttls_cipher_info_t *cipher_info;

	/** Key length to use. */
	int key_bitlen;

	/** Operation that the key of the context has been
	 * initialized for.
	 */
	ttls_operation_t operation;

	/** Buffer for input that has not been processed yet. */
	unsigned char unprocessed_data[TTLS_MAX_BLOCK_LENGTH];

	/** Number of Bytes that have not been processed yet. */
	size_t unprocessed_len;

	/** Current IV or NONCE_COUNTER for CTR-mode. */
	unsigned char iv[TTLS_MAX_IV_LENGTH];

	/** IV size in Bytes, for ciphers with variable-length IVs. */
	size_t iv_size;

	/** The cipher-specific context. */
	void *cipher_ctx;

#if defined(TTLS_CMAC_C)
	/** CMAC-specific context. */
	ttls_cmac_context_t *cmac_ctx;
#endif
} ttls_cipher_context_t;

/**
 * \brief This function retrieves the list of ciphers supported by the generic
 * cipher module.
 *
 * \return	  A statically-allocated array of ciphers. The last entry
 *			  is zero.
 */
const int *ttls_cipher_list(void);

/**
 * \brief			   This function retrieves the cipher-information
 *		  structure associated with the given cipher name.
 *
 * \param cipher_name   Name of the cipher to search for.
 *
 * \return			  The cipher information structure associated with the
 *		  given \p cipher_name, or NULL if not found.
 */
const ttls_cipher_info_t *ttls_cipher_info_from_string(const char *cipher_name);

/**
 * \brief			   This function retrieves the cipher-information
 *		  structure associated with the given cipher type.
 *
 * \param cipher_type   Type of the cipher to search for.
 *
 * \return			  The cipher information structure associated with the
 *		  given \p cipher_type, or NULL if not found.
 */
const ttls_cipher_info_t *ttls_cipher_info_from_type(const ttls_cipher_type_t cipher_type);

/**
 * \brief			   This function retrieves the cipher-information
 *		  structure associated with the given cipher ID,
 *		  key size and mode.
 *
 * \param cipher_id	 The ID of the cipher to search for. For example,
 *		  #TTLS_CIPHER_ID_AES.
 * \param key_bitlen	The length of the key in bits.
 * \param mode		  The cipher mode. For example, #TTLS_MODE_CBC.
 *
 * \return			  The cipher information structure associated with the
 *		  given \p cipher_id, or NULL if not found.
 */
const ttls_cipher_info_t *ttls_cipher_info_from_values(const ttls_cipher_id_t cipher_id,
		  int key_bitlen,
		  const ttls_cipher_mode_t mode);

/**
 * \brief			   This function initializes a \p cipher_context as NONE.
 */
void ttls_cipher_init(ttls_cipher_context_t *ctx);

/**
 * \brief			   This function frees and clears the cipher-specific
 *		  context of \p ctx. Freeing \p ctx itself remains the
 *		  responsibility of the caller.
 */
void ttls_cipher_free(ttls_cipher_context_t *ctx);


/**
 * \brief			   This function initializes and fills the cipher-context
 *		  structure with the appropriate values. It also clears
 *		  the structure.
 *
 * \param ctx		   The context to initialize. May not be NULL.
 * \param cipher_info   The cipher to use.
 *
 * \return			  \c 0 on success,
 *		  #TTLS_ERR_CIPHER_BAD_INPUT_DATA on parameter failure,
 *		  #TTLS_ERR_CIPHER_ALLOC_FAILED if allocation of the
 *		  cipher-specific context failed.
 *
 * \internal Currently, the function also clears the structure.
 * In future versions, the caller will be required to call
 * ttls_cipher_init() on the structure first.
 */
int ttls_cipher_setup(ttls_cipher_context_t *ctx, const ttls_cipher_info_t *cipher_info);

/**
 * \brief		This function returns the block size of the given cipher.
 *
 * \param ctx	The context of the cipher. Must be initialized.
 *
 * \return	   The size of the blocks of the cipher, or zero if \p ctx
 *			   has not been initialized.
 */
static inline unsigned int ttls_cipher_get_block_size(const ttls_cipher_context_t *ctx)
{
	if (NULL == ctx || NULL == ctx->cipher_info)
		return 0;

	return ctx->cipher_info->block_size;
}

/**
 * \brief		This function returns the mode of operation for
 *			   the cipher. For example, TTLS_MODE_CBC.
 *
 * \param ctx	The context of the cipher. Must be initialized.
 *
 * \return	   The mode of operation, or #TTLS_MODE_NONE if
 *			   \p ctx has not been initialized.
 */
static inline ttls_cipher_mode_t ttls_cipher_get_cipher_mode(const ttls_cipher_context_t *ctx)
{
	if (NULL == ctx || NULL == ctx->cipher_info)
		return TTLS_MODE_NONE;

	return ctx->cipher_info->mode;
}

/**
 * \brief	   This function returns the size of the IV or nonce
 *			  of the cipher, in Bytes.
 *
 * \param ctx   The context of the cipher. Must be initialized.
 *
 * \return	  <ul><li>If no IV has been set: the recommended IV size.
 *			  0 for ciphers not using IV or nonce.</li>
 *			  <li>If IV has already been set: the actual size.</li></ul>
 */
static inline int ttls_cipher_get_iv_size(const ttls_cipher_context_t *ctx)
{
	if (NULL == ctx || NULL == ctx->cipher_info)
		return 0;

	if (ctx->iv_size != 0)
		return (int) ctx->iv_size;

	return (int) ctx->cipher_info->iv_size;
}

/**
 * \brief			   This function returns the type of the given cipher.
 *
 * \param ctx		   The context of the cipher. Must be initialized.
 *
 * \return			  The type of the cipher, or #TTLS_CIPHER_NONE if
 *		  \p ctx has not been initialized.
 */
static inline ttls_cipher_type_t ttls_cipher_get_type(const ttls_cipher_context_t *ctx)
{
	if (NULL == ctx || NULL == ctx->cipher_info)
		return TTLS_CIPHER_NONE;

	return ctx->cipher_info->type;
}

/**
 * \brief			   This function returns the name of the given cipher
 *		  as a string.
 *
 * \param ctx		   The context of the cipher. Must be initialized.
 *
 * \return			  The name of the cipher, or NULL if \p ctx has not
 *		  been not initialized.
 */
static inline const char *ttls_cipher_get_name(const ttls_cipher_context_t *ctx)
{
	if (NULL == ctx || NULL == ctx->cipher_info)
		return 0;

	return ctx->cipher_info->name;
}

/**
 * \brief			   This function returns the key length of the cipher.
 *
 * \param ctx		   The context of the cipher. Must be initialized.
 *
 * \return			  The key length of the cipher in bits, or
 *		  #TTLS_KEY_LENGTH_NONE if ctx \p has not been
 *		  initialized.
 */
static inline int ttls_cipher_get_key_bitlen(const ttls_cipher_context_t *ctx)
{
	if (NULL == ctx || NULL == ctx->cipher_info)
		return TTLS_KEY_LENGTH_NONE;

	return (int) ctx->cipher_info->key_bitlen;
}

/**
 * \brief		  This function returns the operation of the given cipher.
 *
 * \param ctx	  The context of the cipher. Must be initialized.
 *
 * \return		 The type of operation: #TTLS_ENCRYPT or
 *				 #TTLS_DECRYPT, or #TTLS_OPERATION_NONE if \p ctx
 *				 has not been initialized.
 */
static inline ttls_operation_t ttls_cipher_get_operation(const ttls_cipher_context_t *ctx)
{
	if (NULL == ctx || NULL == ctx->cipher_info)
		return TTLS_OPERATION_NONE;

	return ctx->operation;
}

/**
 * \brief			   This function sets the key to use with the given context.
 *
 * \param ctx		   The generic cipher context. May not be NULL. Must have
 *		  been initialized using ttls_cipher_info_from_type()
 *		  or ttls_cipher_info_from_string().
 * \param key		   The key to use.
 * \param key_bitlen	The key length to use, in bits.
 * \param operation	 The operation that the key will be used for:
 *		  #TTLS_ENCRYPT or #TTLS_DECRYPT.
 *
 * \returns			 \c 0 on success, #TTLS_ERR_CIPHER_BAD_INPUT_DATA if
 *		  parameter verification fails, or a cipher-specific
 *		  error code.
 */
int ttls_cipher_setkey(ttls_cipher_context_t *ctx, const unsigned char *key,
				   int key_bitlen, const ttls_operation_t operation);

/**
 * \brief		   This function sets the initialization vector (IV)
 *				  or nonce.
 *
 * \param ctx	   The generic cipher context.
 * \param iv		The IV to use, or NONCE_COUNTER for CTR-mode ciphers.
 * \param iv_len	The IV length for ciphers with variable-size IV.
 *				  This parameter is discarded by ciphers with fixed-size IV.
 *
 * \returns		 \c 0 on success, or #TTLS_ERR_CIPHER_BAD_INPUT_DATA
 *
 * \note			Some ciphers do not use IVs nor nonce. For these
 *				  ciphers, this function has no effect.
 */
int ttls_cipher_set_iv(ttls_cipher_context_t *ctx,
				   const unsigned char *iv, size_t iv_len);

/**
 * \brief		 This function resets the cipher state.
 *
 * \param ctx	 The generic cipher context.
 *
 * \returns	   \c 0 on success, #TTLS_ERR_CIPHER_BAD_INPUT_DATA
 *				if parameter verification fails.
 */
int ttls_cipher_reset(ttls_cipher_context_t *ctx);

/**
 * \brief			   This function adds additional data for AEAD ciphers.
 *		  Only supported with GCM. Must be called
 *		  exactly once, after ttls_cipher_reset().
 *
 * \param ctx		   The generic cipher context.
 * \param ad			The additional data to use.
 * \param ad_len		the Length of \p ad.
 *
 * \return			  \c 0 on success, or a specific error code on failure.
 */
int ttls_cipher_update_ad(ttls_cipher_context_t *ctx,
		  const unsigned char *ad, size_t ad_len);

/**
 * \brief			   The generic cipher update function. It encrypts or
 *		  decrypts using the given cipher context. Writes as
 *		  many block-sized blocks of data as possible to output.
 *		  Any data that cannot be written immediately is either
 *		  added to the next block, or flushed when
 *		  ttls_cipher_finish() is called.
 *		  Exception: For TTLS_MODE_ECB, expects a single block
 *		  in size. For example, 16 Bytes for AES.
 *
 * \param ctx		   The generic cipher context.
 * \param input		 The buffer holding the input data.
 * \param ilen		  The length of the input data.
 * \param output		The buffer for the output data. Must be able to hold at
 *		  least \p ilen + block_size. Must not be the same buffer
 *		  as input.
 * \param olen		  The length of the output data, to be updated with the
 *		  actual number of Bytes written.
 *
 * \returns			 \c 0 on success, #TTLS_ERR_CIPHER_BAD_INPUT_DATA if
 *		  parameter verification fails,
 *		  #TTLS_ERR_CIPHER_FEATURE_UNAVAILABLE on an
 *		  unsupported mode for a cipher, or a cipher-specific
 *		  error code.
 *
 * \note				If the underlying cipher is GCM, all calls to this
 *		  function, except the last one before
 *		  ttls_cipher_finish(). Must have \p ilen as a
 *		  multiple of the block_size.
 */
int ttls_cipher_update(ttls_cipher_context_t *ctx, const unsigned char *input,
				   size_t ilen, unsigned char *output, size_t *olen);

/**
 * \brief			   The generic cipher finalization function. If data still
 *		  needs to be flushed from an incomplete block, the data
 *		  contained in it is padded to the size of
 *		  the last block, and written to the \p output buffer.
 *
 * \param ctx		   The generic cipher context.
 * \param output		The buffer to write data to. Needs block_size available.
 * \param olen		  The length of the data written to the \p output buffer.
 *
 * \returns			 \c 0 on success, #TTLS_ERR_CIPHER_BAD_INPUT_DATA if
 *		  parameter verification fails,
 *		  #TTLS_ERR_CIPHER_FULL_BLOCK_EXPECTED if decryption
 *		  expected a full block but was not provided one,
 *		  #TTLS_ERR_CIPHER_INVALID_PADDING on invalid padding
 *		  while decrypting, or a cipher-specific error code
 *		  on failure for any other reason.
 */
int ttls_cipher_finish(ttls_cipher_context_t *ctx,
				   unsigned char *output, size_t *olen);

/**
 * \brief			   This function writes a tag for AEAD ciphers.
 *		  Only supported with GCM.
 *		  Must be called after ttls_cipher_finish().
 *
 * \param ctx		   The generic cipher context.
 * \param tag		   The buffer to write the tag to.
 * \param tag_len	   The length of the tag to write.
 *
 * \return			  \c 0 on success, or a specific error code on failure.
 */
int ttls_cipher_write_tag(ttls_cipher_context_t *ctx,
		  unsigned char *tag, size_t tag_len);

/**
 * \brief			   This function checks the tag for AEAD ciphers.
 *		  Only supported with GCM.
 *		  Must be called after ttls_cipher_finish().
 *
 * \param ctx		   The generic cipher context.
 * \param tag		   The buffer holding the tag.
 * \param tag_len	   The length of the tag to check.
 *
 * \return			  \c 0 on success, or a specific error code on failure.
 */
int ttls_cipher_check_tag(ttls_cipher_context_t *ctx,
		  const unsigned char *tag, size_t tag_len);

/**
 * \brief			   The generic all-in-one encryption/decryption function,
 *		  for all ciphers except AEAD constructs.
 *
 * \param ctx		   The generic cipher context.
 * \param iv			The IV to use, or NONCE_COUNTER for CTR-mode ciphers.
 * \param iv_len		The IV length for ciphers with variable-size IV.
 *		  This parameter is discarded by ciphers with fixed-size
 *		  IV.
 * \param input		 The buffer holding the input data.
 * \param ilen		  The length of the input data.
 * \param output		The buffer for the output data. Must be able to hold at
 *		  least \p ilen + block_size. Must not be the same buffer
 *		  as input.
 * \param olen		  The length of the output data, to be updated with the
 *		  actual number of Bytes written.
 *
 * \note				Some ciphers do not use IVs nor nonce. For these
 *		  ciphers, use \p iv = NULL and \p iv_len = 0.
 *
 * \returns			 \c 0 on success, or
 *		  #TTLS_ERR_CIPHER_BAD_INPUT_DATA, or
 *		  #TTLS_ERR_CIPHER_FULL_BLOCK_EXPECTED if decryption
 *		  expected a full block but was not provided one, or
 *		  #TTLS_ERR_CIPHER_INVALID_PADDING on invalid padding
 *		  while decrypting, or a cipher-specific error code on
 *		  failure for any other reason.
 */
int ttls_cipher_crypt(ttls_cipher_context_t *ctx,
				  const unsigned char *iv, size_t iv_len,
				  const unsigned char *input, size_t ilen,
				  unsigned char *output, size_t *olen);

/**
 * \brief			   The generic autenticated encryption (AEAD) function.
 *
 * \param ctx		   The generic cipher context.
 * \param iv			The IV to use, or NONCE_COUNTER for CTR-mode ciphers.
 * \param iv_len		The IV length for ciphers with variable-size IV.
 *		  This parameter is discarded by ciphers with fixed-size IV.
 * \param ad			The additional data to authenticate.
 * \param ad_len		The length of \p ad.
 * \param input		 The buffer holding the input data.
 * \param ilen		  The length of the input data.
 * \param output		The buffer for the output data.
 *		  Must be able to hold at least \p ilen.
 * \param olen		  The length of the output data, to be updated with the
 *		  actual number of Bytes written.
 * \param tag		   The buffer for the authentication tag.
 * \param tag_len	   The desired length of the authentication tag.
 *
 * \returns			 \c 0 on success, or
 *		  #TTLS_ERR_CIPHER_BAD_INPUT_DATA, or
 *		  a cipher-specific error code.
 */
int ttls_cipher_auth_encrypt(ttls_cipher_context_t *ctx,
			 const unsigned char *iv, size_t iv_len,
			 const unsigned char *ad, size_t ad_len,
			 const unsigned char *input, size_t ilen,
			 unsigned char *output, size_t *olen,
			 unsigned char *tag, size_t tag_len);

/**
 * \brief			   The generic autenticated decryption (AEAD) function.
 *
 * \param ctx		   The generic cipher context.
 * \param iv			The IV to use, or NONCE_COUNTER for CTR-mode ciphers.
 * \param iv_len		The IV length for ciphers with variable-size IV.
 *		  This parameter is discarded by ciphers with fixed-size IV.
 * \param ad			The additional data to be authenticated.
 * \param ad_len		The length of \p ad.
 * \param input		 The buffer holding the input data.
 * \param ilen		  The length of the input data.
 * \param output		The buffer for the output data.
 *		  Must be able to hold at least \p ilen.
 * \param olen		  The length of the output data, to be updated with the
 *		  actual number of Bytes written.
 * \param tag		   The buffer holding the authentication tag.
 * \param tag_len	   The length of the authentication tag.
 *
 * \returns			 \c 0 on success, or
 *		  #TTLS_ERR_CIPHER_BAD_INPUT_DATA, or
 *		  #TTLS_ERR_CIPHER_AUTH_FAILED if data is not authentic,
 *		  or a cipher-specific error code on failure for any other reason.
 *
 * \note				If the data is not authentic, then the output buffer
 *		  is zeroed out to prevent the unauthentic plaintext being
 *		  used, making this interface safer.
 */
int ttls_cipher_auth_decrypt(ttls_cipher_context_t *ctx,
			 const unsigned char *iv, size_t iv_len,
			 const unsigned char *ad, size_t ad_len,
			 const unsigned char *input, size_t ilen,
			 unsigned char *output, size_t *olen,
			 const unsigned char *tag, size_t tag_len);

#endif /* TTLS_CIPHER_H */
