#ifndef VERIFICATION_H_
#define VERIFICATION_H_

#include "ksi.h"

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * TODO!
	 */
	typedef struct KSI_VerificationInfo_st KSI_VerificationResult;
	typedef struct KSI_VerificationStepResult_st KSI_VerificationStepResult;

	typedef enum KSI_VerificationStep_en {
	    /**
	     * OK! verify aggregation chain
	     */
	    KSI_SIG_AGGREGATION_CHAIN = 0x01,
	    /**
	     * OK!check if signature and document hashes match
	     */
	    KSI_SIG_DOCUMENT_MATCH = 0x02,
	    /**
	     * OK! check if calendar chain matches aggregation chain
	     */
	    KSI_SIG_CALENDAR_CHAIN = 0x04,
	    /**
	     * check publication file signature
	     */
	    KSI_PUB_SIGNATURE = 0x08,
	    /**
	     * OK!verify that calendar authentication record matches calendar chain
	     */
	    KSI_SIG_CAL_AUTH_REC_MATCH = 0x10,
	    /**
	     * OK!verify that calendar authentication record signature is correct
	     */
	    KSI_SIG_CAL_AUTH_REC_SIGNATURE = 0x20,
	    /**
	     * OK! check if publication record matches calendar chain
	     */
	    KSI_SIG_PUBLICATION_MATCH = 0x40,
	    /**
	     * OK! check if publication record is stored in KSI Trust provider
	     */
	    KSI_SIG_PUBLICATION_TRUSTED = 0x80,
	    /**
	     * OK! verify signature against online calendar
	     */
	    KSI_SIG_VERIFY_ONLINE = 0x100
	} KSI_VerificationStep;

	/**
	 * TODO!
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationResult_init(KSI_VerificationResult *info);

	/**
	 * TODO!
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationResult_reset(KSI_VerificationResult *info);

	/**
	 * TODO!
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationResult_addFailure(KSI_VerificationResult *info, KSI_VerificationStep step, const char *desc);

	/**
	 * TODO!
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationResult_addSuccess(KSI_VerificationResult *info, KSI_VerificationStep step, const char *desc);

	/**
	 * Returns the performed step count.
	 * \param[in]	info		Verification result.
	 * \return count of elements in the verification info.
	 */
	size_t KSI_VerificationResult_getStepResultCount(const KSI_VerificationResult *info);

	/**
	 * Get the a verification step with the given index.
	 * \param[in]	info		Verification result.
	 * \param[in]	index		Index of the step.
	 * \param[out]	step		Verification step objec.
	 *
	 * \return status code (#KSI_OK, when operation succeeded, otherwise an error code).
	 */
	int KSI_VerificationResult_getStepResult(const KSI_VerificationResult *info, size_t index, const KSI_VerificationStepResult **result);

	/**
	 * Returns 0 if the given verification step is not performed.
	 * \param[in]	info		Verification result.
	 * \param[in]	step		Verification step.
	 * \return 0 is the given verification step is not performed.
	 */
	int KSI_VerificationResult_isStepPerformed(const KSI_VerificationResult *info, enum KSI_VerificationStep_en step);

	/**
	 * Returns 0 if the given verification step is not performed or the performed step was
	 * unsuccessful.
	 * \param[in]	info		Verification result.
	 * \param[in]	step		Verification step.
	 * \returns 0 if the given verification step was unsuccessful or not performed.
	 */
	int KSI_VerificationResult_isStepSuccess(const KSI_VerificationResult *info, enum KSI_VerificationStep_en step);

	/**
	 * Returns the #KSI_VerificationStep value or 0 on an error.
	 * \param[in]	retult 		Verification step result.
	 */
	int KSI_VerificationStepResult_getStep(const KSI_VerificationStepResult *result);

	/**
	 * Returns if the verification step result was successful.
	 * \param[in]	result		Verification step result.
	 * \return If the step was not successful 0 is returned, otherwise !0.
	 */
	int KSI_VerificationStepResult_isSuccess(const KSI_VerificationStepResult *result);

	/**
	 * Returns a pointer to the description of this step result.
	 * \param[in]	result		Verification step result.
	 */
	const char *KSI_VerificationStepResult_getDescription(const KSI_VerificationStepResult *result);

#ifdef __cplusplus
}
#endif

#endif /* VERIFICATION_INFO_H_ */
