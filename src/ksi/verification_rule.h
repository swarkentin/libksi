/*
 * Copyright 2013-2016 Guardtime, Inc.
 *
 * This file is part of the Guardtime client SDK.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES, CONDITIONS, OR OTHER LICENSES OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 * "Guardtime" and "KSI" are trademarks or registered trademarks of
 * Guardtime, Inc., and no license to trademarks is granted; Guardtime
 * reserves and retains all trademark rights.
 */

#ifndef VERIFICATION_RULE_H_
#define VERIFICATION_RULE_H_

#include "ksi.h"
#include "policy.h"

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * This rule verifies that the document input level (default 0) is greater than the initial level correction
	 * (always 0 for RFC-3161 record) of the first hash chain.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_AggregationChainInputLevelVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule is used to verify if the hash algorithm of the input hash of the signature (input hash of the first
	 * aggregation hash chain, or if present, the input hash of the RFC-3161 record) was deprecated at the
	 * aggregation time (i.e aggregation time in the current record).
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_AggregationChainInputHashAlgorithmVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule for verifying RFC-3161 record absence.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_Rfc3161DoesNotExist(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule for verifying RFC-3161 record presence.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_Rfc3161Existence(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule is used to verify if the RFC-3161 record output hash algorithm (taken from the input hash from the
	 * first aggregation hash chain) was deprecated at the aggregation time.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_Rfc3161RecordOutputHashAlgorithmVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule is used to verify if the RFC-3161 record uses internally a hash function that was deprecated at the
	 * aggregation time fail.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_Rfc3161RecordHashAlgorithmVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule verifies that if RFC3161 record is present then the calculated output hash (from RFC3161 record)
	 * equals to aggregation chain input hash. If RFC3161 record is missing then the status #KSI_VER_RES_OK is
	 * returned.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_AggregationChainInputHashVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule verifies that the metadata structures contain a valid padding and ensure that metadata cannot be
	 * interpreted as an imprint.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_AggregationChainMetaDataVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule is used to verify if the aggregation hash chain uses a hash algorithm that was deprecated at the
	 * aggregation time to aggregate the sibling hashes.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_AggregationChainHashAlgorithmVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule verifies that all aggregation hash chains are consistent (e.g, previous aggregation output hash
	 * equals to current aggregation chain input hash)
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_AggregationHashChainConsistency(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check that aggregation hash chain aggregation times are consistent (e.g previous aggregation
	 * hash chain aggregation time to current aggregation hash chain aggregation time).
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_AggregationHashChainTimeConsistency(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check whether the aggregation hash chain chain index continuation is consistent.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_AggregationHashChainIndexContinuation(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check whether the shape of the aggregation hash chain does match with the chain index.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_AggregationHashChainIndexConsistency(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify that last aggregation hash chain output hash equals to calendar hash chain input hash.
	 * If calendar hash chain is missing then status code #KSI_OK will be returned.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CalendarHashChainInputHashVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify that calendar hash chain aggregation time equals to last aggregation hash chain
	 * aggregation time. If calendar hash chain is missing then status code #KSI_OK is returned.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CalendarHashChainAggregationTime(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify that calendar hash chain registration time (calculated from the shape of the calendar
	 * hash chain) equals to calendar hash chain aggregation time. If calendar hash chain is missing then status code
	 * #KSI_OK will be returned.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CalendarHashChainRegistrationTime(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule is used to verify if any of the calendar hash chain aggregation (derived from the right link) hash
	 * algorithms were obsolete at the publication time.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 *
	 * \note In case of an invalid signature the error code in verification result is set to #KSI_VER_ERR_INT_16
	 */
	int KSI_VerificationRule_CalendarChainHashAlgorithmObsoleteAtPubTime(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify that calendar authentication record publication hash equals to calendar hash chain
	 * publication hash. If calendar authentication record is missing then status code #KSI_OK is returned.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CalendarAuthenticationRecordAggregationHash(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify that calendar authentication record publication time equals to calendar hash chain
	 * publication time. If calendar authentication record is missing then status code #KSI_OK is returned.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CalendarAuthenticationRecordAggregationTime(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check if the signature contains publication record or not. If publication record is
	 * missing then status code #KSI_OK will be returned.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_SignaturePublicationRecordPublicationHash(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check if the signature contains correct publication record publication time.
	 * If publication record is missing then status code #KSI_OK will ne returned.

	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_SignaturePublicationRecordPublicationTime(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule to check that document hash has not been provided for verification.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_DocumentHashDoesNotExist(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check if document hash has been provided for verification.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_DocumentHashExistence(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify input hash algorithm. If document hash is provided, it's hash algorithm must
	 * match with the hash algorithm of the input hash of the first aggragation chain or RFC-3161 record if
	 * present.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_InputHashAlgorithmVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify document hash. If RFC3161 record is present then the document hash must equal to RFC3161
	 * input hash. If RFC3161 record isn't present then document hash must equal to first aggregation hash chain input hash.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_DocumentHashVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule checks that signature does not contain publication record.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_SignatureDoesNotContainPublication(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule checks that:
	 * - the extended signature contains the same count of right calendar hash chain links
	 * - the extended signature right calendar hash chain links are equal to the not extended signature right links
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_ExtendedSignatureCalendarChainRightLinksMatch(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check if the signature contains publication record or not.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_SignaturePublicationRecordExistence(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check if publication record is missing.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 * \see #KSI_VerificationRule_SignaturePublicationRecordExistence for opposite result.
	 */
	int KSI_VerificationRule_SignaturePublicationRecordMissing(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check that reproduced calendar hash chain (reproduced by sending extension request with the
	 * same aggregation and publication time as the attached calendar chain) matches with the already present calendar
	 * hash chain root hash.
	 * If signature (that is being validated), does not contain calendar hash chain then status code #KSI_OK will be
	 * returned.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_ExtendedSignatureCalendarChainRootHash(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule to check that the signature does not contain calendar hash chain.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 * \see KSI_VerificationRule_CalendarHashChainExistence for opposite result.
	 */
	int KSI_VerificationRule_CalendarHashChainDoesNotExist(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check that extended signature contains correct calendar hash chain input hash (e.g  matches
	 * with aggregation chain root hash).
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_ExtendedSignatureCalendarChainInputHash(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check that extended signature contains correct aggregation time.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_ExtendedSignatureCalendarChainAggregationTime(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule to check if the signature contains calendar hash chain.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 * \see KSI_VerificationRule_CalendarHashChainDoesNotExist for opposite result.
	 */
	int KSI_VerificationRule_CalendarHashChainExistence(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule for verification of calendar hash chain presence in KSI signature. Used by key-based verification policy.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 * \see KSI_VerificationRule_CalendarHashChainDoesNotExist for opposite result.
	 */
	int KSI_VerificationRule_CalendarHashChainPresenceVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule is used to verify if any of the calendar hash chain aggregation hash algorithms (derived from the
	 * right link) were deprecated at the publication time.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CalendarHashChainHashAlgorithmDeprecatedAtPubTime(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule for verifying calendar authentication record existence.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CalendarAuthenticationRecordExistence(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule for verifying calendar authentication record absence.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CalendarAuthenticationRecordDoesNotExist(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule for calendar authentication record presence verification.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CalendarAuthenticationRecordPresenceVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule ise used to check if publications file contains certificate with certificate id present in calendar
	 * authentication record.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CertificateExistence(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to validate calendar authentication record signature lifespan during KSI signature acquisition.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CertificateValidity(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to validate calendar authentication record signature. At first X.509 certificate is searched from
	 * publications file and when the certificate is found then the PKI signature is validated.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_CalendarAuthenticationRecordSignatureVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);


	/**
	 * This rule can be used to check if publications file contains signature publication.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 * \see KSI_VerificationRule_PublicationsFileDoesNotContainSignaturePublication for opposite result.
	 */
	int KSI_VerificationRule_PublicationsFileContainsSignaturePublication(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule can be used to check if signature publication record is missing from publications file.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 * \see KSI_VerificationRule_PublicationsFileContainsSignaturePublication for opposite result.
	 */
	int KSI_VerificationRule_PublicationsFileDoesNotContainSignaturePublication(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check if the hashes of the publications file publication record and the signature publication record match.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_PublicationsFileSignaturePublicationVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to check if publications file contains publication closest to signature registration time.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_PublicationsFileContainsSuitablePublication(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule can be used to check if signature extending is permitted or not.
	 * Rule is used by publications file based policy.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_PublicationsFileExtendingPermittedVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule can be used to check if signature extending is permitted or not.
	 * Rule is used by user provided publication based policy.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_UserProvidedPublicationExtendingPermittedVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule is used to verify if any of the calendar hash chain aggregation hash algorithms (derived from the
	 * right link) were deprecated at the publication time.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_PublicationsFileSignatureCalendarChainHashAlgorithmDeprecatedAtPubTime(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule is used to verify if any of the calendar hash chain aggregation hash algorithms (derived from the
	 * right link) were deprecated at the publication time.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_UserProvidedPublicationSignatureCalendarChainHashAlgorithmDeprecatedAtPubTime(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule is used to verify if any of the extended calendar hash chain aggregation hash algorithms (derived from the
	 * right link) were deprecated at the publication time.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_PublicationsFileExtendedCalendarChainHashAlgorithmDeprecatedAtPubTime(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify that publications file publication hash matches with extender response calendar root hash.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_PublicationsFilePublicationHashMatchesExtenderResponse(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify that publications file publication time matches with extender response calendar chain
	 * shape.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_PublicationsFilePublicationTimeMatchesExtenderResponse(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule can be used to check that extender response input hash equals with signature aggregation root hash.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_PublicationsFileExtendedSignatureInputHash(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify if user has provided the publication
	 *
	 * \param[in]	info		Verification context to be used for given rule.
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_UserProvidedPublicationExistence(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify that the user has NOT provided a publication.
	 *
	 * \param[in]	info		Verification context to be used for given rule.
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_RequireNoUserProvidedPublication(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used verify that user provided publication time equals to publication time inside the signature.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 * \see KSI_VerificationRule_UserProvidedPublicationTimeDoesNotSuit for opposite result.
	 */
	int KSI_VerificationRule_UserProvidedPublicationTimeVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify that user provided publication time does not equals to publication time inside the signature.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 * \see KSI_VerificationRule_UserProvidedPublicationTimeVerification for opposite result.
	 */
	int KSI_VerificationRule_UserProvidedPublicationTimeDoesNotSuit(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used verify that user provided publication hash equals to publication hash inside the signature.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_UserProvidedPublicationHashVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule checks that signature is created before user provided publication.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_UserProvidedPublicationCreationTimeVerification(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * Rule is used to verify if any of the extended calendar hash chain aggregation hash algorithms (derived from the
	 * right link) were deprecated at the publication time.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_UserProvidedPublicationExtendedCalendarChainHashAlgorithmDeprecatedAtPubTime(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify that user provided publication hash matches with extender response calendar root hash
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_UserProvidedPublicationHashMatchesExtendedResponse(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule is used to verify that user provided publication time matches with extender response calendar chain shape.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_UserProvidedPublicationTimeMatchesExtendedResponse(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

	/**
	 * This rule can be used to check that extender response input hash equals with signature aggregation root hash.
	 *
	 * \param[in]	info		Verification context to be used for given rule
	 * \param[out]	result		Verification result.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationRule_UserProvidedPublicationExtendedSignatureInputHash(KSI_VerificationContext *info, KSI_RuleVerificationResult *result);

#ifdef __cplusplus
}
#endif

#endif /* VERIFICATION_RULE_H_ */
