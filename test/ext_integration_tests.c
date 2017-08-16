/*
 * Copyright 2013-2015 Guardtime, Inc.
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

#include "cutest/CuTest.h"
#include "all_integration_tests.h"
#include <ksi/net_http.h>
#include <ksi/net_uri.h>
#include <ksi/net.h>
#include <ksi/net_tcp.h>
#include "../src/ksi/internal.h"

extern KSI_CTX *ctx;
extern KSITest_Conf conf;

static void postTest(void) {
	/* Restore default PDU version. */
	KSI_CTX_setFlag(ctx, KSI_OPT_EXT_PDU_VER, (void*)KSI_EXTENDING_PDU_VERSION);

	KSI_CTX_setExtender(ctx, KSITest_composeUri("ksi+http", &conf.extender), conf.extender.user, conf.extender.pass);
}

static void getExtResponse(CuTest* tc, KSI_uint64_t id, KSI_uint64_t aggrTime, KSI_uint64_t pubTime, KSI_ExtendResp **response) {
	int res;
	KSI_ExtendReq *request = NULL;
	KSI_Integer *ID = NULL;
	KSI_Integer *aggr_time = NULL;
	KSI_Integer *pub_time = NULL;
	KSI_RequestHandle *handle = NULL;
	KSI_ExtendResp *tmp = NULL;


	KSI_ERR_clearErrors(ctx);

	/*Create objects*/
	res = KSI_ExtendReq_new(ctx, &request);
	CuAssert(tc, "Unable to create extend request.", res == KSI_OK && request != NULL);

	res = KSI_Integer_new(ctx, id, &ID);
	CuAssert(tc, "Unable to create request ID.", res == KSI_OK && ID != NULL);

	res = KSI_Integer_new(ctx, aggrTime, &aggr_time);
	CuAssert(tc, "Unable to aggr time.", res == KSI_OK && aggr_time != NULL);

	res = KSI_Integer_new(ctx, pubTime, &pub_time);
	CuAssert(tc, "Unable to pub time.", res == KSI_OK && pub_time != NULL);


	/*Combine objects*/
	res = KSI_ExtendReq_setRequestId(request, ID);
	CuAssert(tc, "Unable set request ID.", res == KSI_OK);
	ID = NULL;

	res = KSI_ExtendReq_setAggregationTime(request, aggr_time);
	CuAssert(tc, "Unable set aggre time.", res == KSI_OK);
	aggr_time = NULL;

	res = KSI_ExtendReq_setPublicationTime(request, pub_time);
	CuAssert(tc, "Unable set pub time.", res == KSI_OK);
	pub_time = NULL;

	/*Send request and get response*/
	res = KSI_sendExtendRequest(ctx, request, &handle);
	CuAssert(tc, "Unable to send (prepare) sign request.", res == KSI_OK);

	res = KSI_RequestHandle_perform(handle);
	CuAssert(tc, "Unable to send perform (send) sign request.", res == KSI_OK);

	res = KSI_RequestHandle_getExtendResponse(handle, &tmp);
	CuAssert(tc, "Unable to get (send and get) sign request.", res == KSI_OK && tmp != NULL);


	*response = tmp;
	tmp = NULL;
	res = KSI_OK;

	KSI_ExtendReq_free(request);
	KSI_Integer_free(aggr_time);
	KSI_Integer_free(pub_time);
	KSI_Integer_free(ID);
	KSI_ExtendResp_free(tmp);

	KSI_RequestHandle_free(handle);
}

static void Test_SendOKExtendRequestDefProvider(CuTest* tc) {
	int res = KSI_UNKNOWN_ERROR;
	KSI_ExtendResp *response = NULL;
	KSI_Integer *resp_status = NULL;

	KSI_ERR_clearErrors(ctx);

	getExtResponse(tc, 0x01, 1435740789, 1435827189, &response);
	CuAssert(tc, "Unable to send (prepare) sign request.", response != NULL);

	res = KSI_ExtendResp_getStatus(response, &resp_status);
	CuAssert(tc, "Unable to get response status.", res == KSI_OK && resp_status != NULL);
	CuAssert(tc, "Response contains errors.", KSI_Integer_equalsUInt(resp_status, 0));


	KSI_ExtendResp_free(response);

	return;
}

static void Test_OKExtendSignatureDefProvider(CuTest* tc) {
	int res;
	KSI_Signature *sig = NULL;
	KSI_Signature *ext = NULL;

	res = KSI_Signature_fromFile(ctx, getFullResourcePath("resource/tlv/ok-sig-2014-07-01.1.ksig"), &sig);
	CuAssert(tc, "Unable to read signature frome file.", res == KSI_OK && sig != NULL);

	res = KSI_Signature_extend(sig, ctx, NULL, &ext);
	CuAssert(tc, "Unable to extend signature", res == KSI_OK && ext != NULL);

	res = KSI_verifySignature(ctx, sig);
	CuAssert(tc, "Unable to verify signature", res == KSI_OK);

	KSI_ERR_clearErrors(ctx);

	KSI_Signature_free(sig);
	KSI_Signature_free(ext);
}

static void Test_NOKExtendRequestToTheFuture(CuTest* tc) {
	int res = KSI_UNKNOWN_ERROR;
	KSI_ExtendResp *response = NULL;
	KSI_Integer *resp_status = NULL;

	KSI_ERR_clearErrors(ctx);
	getExtResponse(tc, 0x01, 1435740789, 2435827189, &response);
	CuAssert(tc, "Unable to send (prepare) sign request.", response != NULL);

	res = KSI_ExtendResp_getStatus(response, &resp_status);
	CuAssert(tc, "Unable to get response status.", res == KSI_OK && resp_status != NULL);
	CuAssert(tc, "Wrong error.", KSI_Integer_equalsUInt(resp_status, 0x107));


	KSI_ExtendResp_free(response);

	return;
}

static void Test_NOKExtendRequestToPast(CuTest* tc) {
	int res = KSI_UNKNOWN_ERROR;
	KSI_ExtendResp *response = NULL;
	KSI_Integer *resp_status = NULL;

	KSI_ERR_clearErrors(ctx);
	getExtResponse(tc, 0x01, 2435827189, 1435740789, &response);
	CuAssert(tc, "Unable to send (prepare) sign request.", response != NULL);

	res = KSI_ExtendResp_getStatus(response, &resp_status);
	CuAssert(tc, "Unable to get response status.", res == KSI_OK && resp_status != NULL);
	CuAssert(tc, "Wrong error.", KSI_Integer_equalsUInt(resp_status, 0x104));


	KSI_ExtendResp_free(response);

	return;
}

static void Test_ExtendSignatureUsingAggregator(CuTest* tc) {
	int res = KSI_UNKNOWN_ERROR;
	KSI_Signature *sig = NULL;
	KSI_Signature *ext = NULL;
	KSI_CTX *ctx = NULL;

	/* Create the context. */
	res = KSI_CTX_new(&ctx);
	CuAssert(tc, "Unable to create ctx.", res == KSI_OK && ctx != NULL);

	res = KSI_CTX_setPublicationUrl(ctx, conf.publications_file_url);
	CuAssert(tc, "Unable to set publications file url.", res == KSI_OK);

	res = KSI_CTX_setExtender(ctx, KSITest_composeUri("ksi+http", &conf.aggregator), conf.aggregator.user, conf.aggregator.pass);
	CuAssert(tc, "Unable to set configure aggregator as extender.", res == KSI_OK);

	res = KSI_Signature_fromFile(ctx, getFullResourcePath("resource/tlv/ok-sig-2014-07-01.1.ksig"), &sig);
	CuAssert(tc, "Unable to set read signature from file.", res == KSI_OK && sig != NULL);

	res = KSI_Signature_extend(sig, ctx, NULL, &ext);
	CuAssert(tc, "The extending of signature must fail.", ext == NULL);
	CuAssert(tc, "Invalid KSI status code for mixed up request.", res == KSI_HTTP_ERROR);
	CuAssert(tc, "External error (HTTP) must be 400.", ctx_get_base_external_error(ctx) == 400);


	KSI_Signature_free(sig);
	KSI_Signature_free(ext);
	KSI_CTX_free(ctx);
	return;
}

static void Test_ExtendSignature_useProvider(CuTest* tc, const KSITest_ServiceConf *service, const char *pub_uri,
		int (*createProvider)(KSI_CTX *ctx, KSI_NetworkClient **http),
		int (*setPubfail)(KSI_NetworkClient *client, const char *url),
		int (*setExtender)(KSI_NetworkClient *client, const KSITest_ServiceConf *service)) {
	int res = KSI_UNKNOWN_ERROR;
	KSI_Signature *sig = NULL;
	KSI_Signature *ext = NULL;
	KSI_NetworkClient *client = NULL;
	KSI_CTX *ctx = NULL;

	/* Create the context. */
	res = KSI_CTX_new(&ctx);
	CuAssert(tc, "Unable to create ctx.", res == KSI_OK && ctx != NULL);

	res = createProvider(ctx, &client);
	CuAssert(tc, "Unable to create network client.", res == KSI_OK && client != NULL);

	res = setExtender(client, service);
	CuAssert(tc, "Unable to set extender specific service information.", res == KSI_OK);

	res = setPubfail(client, pub_uri);
	CuAssert(tc, "Unable to set publications file url.", res == KSI_OK);

	res = KSI_CTX_setNetworkProvider(ctx, client);
	CuAssert(tc, "Unable to set new network client.", res == KSI_OK);
	client = NULL;

	res = KSI_Signature_fromFile(ctx, getFullResourcePath("resource/tlv/ok-sig-2014-07-01.1.ksig"), &sig);
	CuAssert(tc, "Unable to set read signature from file.", res == KSI_OK && sig != NULL);

	res = KSI_Signature_extend(sig, ctx, NULL, &ext);
	CuAssert(tc, "The extending of signature must not fail.", res == KSI_OK && ext != NULL);

	KSI_NetworkClient_free(client);
	KSI_Signature_free(sig);
	KSI_Signature_free(ext);
	KSI_CTX_free(ctx);
	return;
}

static int uriHttp_setExtWrapper(KSI_NetworkClient *client, const KSITest_ServiceConf *service) {
	return KSI_UriClient_setExtender(client, KSITest_composeUri("ksi+http", service), service->user, service->pass);
}

static int uriHttp_setExtWrapper_noCred(KSI_NetworkClient *client, const KSITest_ServiceConf *service) {
	return KSI_UriClient_setExtender(client, KSITest_composeUri("ksi+http", service), NULL, NULL);
}

static int uriTcp_setExtWrapper_noCred(KSI_NetworkClient *client, const KSITest_ServiceConf *service) {
	return KSI_UriClient_setExtender(client, KSITest_composeUri("ksi+tcp", service), NULL, NULL);
}

static int tcp_setExtWrapper(KSI_NetworkClient *client, const KSITest_ServiceConf *service) {
	return KSI_TcpClient_setExtender(client, service->host, service->port, service->user, service->pass);
}


static void Test_ExtendSignatureDifferentNetProviders(CuTest* tc) {
	/* Uri provider HTTP. */
	Test_ExtendSignature_useProvider(tc, &conf.extender, conf.publications_file_url,
			KSI_UriClient_new,
			KSI_UriClient_setPublicationUrl,
			uriHttp_setExtWrapper);
	return;
}

static void Test_ExtendSignatureDifferentNetProviders_Tcp(CuTest* tc) {

	/* Uri provider TCP. */
	Test_ExtendSignature_useProvider(tc, &conf.extender, conf.publications_file_url,
			KSI_TcpClient_new,
			KSI_TcpClient_setPublicationUrl,
			tcp_setExtWrapper);
	return;
}

static void Test_ExtendSignatureUserInfoFromUrl(CuTest* tc) {
	/* Uri provider - all info is extracted from uri. */
	Test_ExtendSignature_useProvider(tc, &conf.extender, conf.publications_file_url,
			KSI_UriClient_new,
			KSI_UriClient_setPublicationUrl,
			uriHttp_setExtWrapper_noCred);
	return;
}

static void Test_ExtendSignatureUserInfoFromUrl_Tcp(CuTest* tc) {
	/* Uri provider - all info is extracted from uri. */
	Test_ExtendSignature_useProvider(tc, &conf.extender, conf.publications_file_url,
			KSI_UriClient_new,
			KSI_UriClient_setPublicationUrl,
			uriTcp_setExtWrapper_noCred);
	return;
}

static void Test_RequestExtenderConfig(CuTest* tc) {
	int res = KSI_UNKNOWN_ERROR;
	KSI_Config *config = NULL;

	KSI_CTX_setFlag(ctx, KSI_OPT_EXT_PDU_VER, (void*)KSI_PDU_VERSION_1);

	res = KSI_receiveExtenderConfig(ctx, &config);
	CuAssert(tc, "Unable to receive extender config.", res == KSI_UNSUPPORTED_PDU_VERSION && config == NULL);
}

static void Test_RequestExtenderConfig_pduV2(CuTest* tc) {
	int res = KSI_UNKNOWN_ERROR;
	KSI_Config *config = NULL;

	KSI_CTX_setFlag(ctx, KSI_OPT_EXT_PDU_VER, (void*)KSI_PDU_VERSION_2);

	res = KSI_receiveExtenderConfig(ctx, &config);
	CuAssert(tc, "Unable to receive extender config.", res == KSI_OK && config != NULL);

	KSI_Config_free(config);
}


CuSuite* ExtIntegrationTests_getSuite(void) {
	CuSuite* suite = CuSuiteNew();

	suite->postTest = postTest;

	SUITE_ADD_TEST(suite, Test_SendOKExtendRequestDefProvider);
	SUITE_ADD_TEST(suite, Test_NOKExtendRequestToTheFuture);
	SUITE_ADD_TEST(suite, Test_NOKExtendRequestToPast);
	SUITE_ADD_TEST(suite, Test_OKExtendSignatureDefProvider);
	SUITE_ADD_TEST(suite, Test_ExtendSignatureUsingAggregator);
	SUITE_ADD_TEST(suite, Test_ExtendSignatureDifferentNetProviders);
	SUITE_SKIP_TEST(suite, Test_ExtendSignatureDifferentNetProviders_Tcp, "Max", "Waiting for gateway release.");
	SUITE_ADD_TEST(suite, Test_ExtendSignatureUserInfoFromUrl);
	SUITE_SKIP_TEST(suite, Test_ExtendSignatureUserInfoFromUrl_Tcp, "Max", "Waiting for gateway release.");
	SUITE_ADD_TEST(suite, Test_RequestExtenderConfig);
	SUITE_ADD_TEST(suite, Test_RequestExtenderConfig_pduV2);

	return suite;
}


