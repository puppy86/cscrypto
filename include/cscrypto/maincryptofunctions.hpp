#ifndef CSCRYPTO_MAINCRYPTOFUNCTIONS_HPP
#define CSCRYPTO_MAINCRYPTOFUNCTIONS_HPP

#include "cscrypto/privatekey.hpp"

namespace cscrypto {
/**
 *   @brief Does the job for the correct work of the crypto library.
 *
 *   Should be called before any other crypto function.
 *   @return true if initialization successful, false if error.
 */

bool cryptoInit();

/**
 *   @brief Calculates hash from byte array of arbitrary size.
 *
 *   @param[in]  data - hash value depends on this.
 *   @return Returns calculated hash.
 *
 *   If you need to calculate MAC (message authentication code)
 *   use two optional params (key and keySize). Two calculated
 *   hashes with the same data but different keys will not match.
 */
Hash calculateHash(const Byte* data, size_t dataSize, const Byte* key = nullptr, size_t keySize = 0);

/**
 *   @brief Generates new crypto keys appropriate to cipher or to sign data.
 */
PrivateKey generateKeyPair(PublicKey& publicKey);

/**
 *   @brief Tries to sign data and verify generated signature with given keys.
 *   @return true if keys are valid, false otherwise.
 */
bool validateKeyPair(const PublicKey& publicKey, const PrivateKey& privateKey);

/**
 *   @brief Returns public key from generated private key.
 */
PublicKey getMatchingPublic(const PrivateKey& privateKey);

/**
 *   @brief Signs data with private key and saves the result in signature.
 *   @return Returns generated signature.
 */
Signature generateSignature(const PrivateKey& privateKey, const Byte* data, size_t dataSize);

/**
 *   @brief Verifies with public key that signature of data was generated by
 *   related private key owner.
 *
 *   @return true if signature is correct, false if incorrect
 */
bool verifySignature(const Signature& signature, const PublicKey& publicKey, const Byte* data, size_t dataSize);

/**
 *   @brief Verifies with public key that signature of data was generated by
 *   related private key owner represented as pointers.
 *
 *   @return true if signature is correct, false if incorrect.
 */
bool verifySignature(const Byte* signature, const Byte* publicKey, const Byte* data, size_t dataSize);

/**
 *   @brief Fills buf_size bytes starting at buf with an unpredictable sequence of bytes.
 */
void fillBufWithRandomBytes(void* buffer, size_t size);
void fillWithZeros(void* buffer, size_t size);

}  // namespace cscrypto
#endif  // CSCRYPTO_MAINCRYPTOFUNCTIONS_HPP
