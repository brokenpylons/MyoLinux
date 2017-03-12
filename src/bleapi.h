/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef BLEAPI_H
#define BLEAPI_H

#include <cinttypes>
#define PACKED __attribute__((__packed__))

struct PACKED Header {
    uint8_t length1 : 3;
    uint8_t tech : 4;
    uint8_t type : 1;
    uint8_t length;
    uint8_t cls;
    uint8_t cmd;
};

struct PACKED SystemReset {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 0;
    uint8_t boot_in_dfu;
};

struct PACKED SystemResetResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 0;
};

struct PACKED SystemHello {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 1;
};

struct PACKED SystemHelloResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 1;
};

struct PACKED SystemAddressGet {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 2;
};

struct PACKED SystemAddressGetResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 2;
    uint8_t address[6];
};

struct PACKED SystemRegWrite {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 3;
    uint16_t address;
    uint8_t value;
};

struct PACKED SystemRegWriteResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 3;
    uint16_t result;
};

struct PACKED SystemRegRead {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 4;
    uint16_t address;
};

struct PACKED SystemRegReadResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 4;
    uint16_t address;
    uint8_t value;
};

struct PACKED SystemGetCounters {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 5;
};

struct PACKED SystemGetCountersResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 5;
    uint8_t txok;
    uint8_t txretry;
    uint8_t rxok;
    uint8_t rxfail;
    uint8_t mbuf;
};

struct PACKED SystemGetConnections {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 6;
};

struct PACKED SystemGetConnectionsResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 6;
    uint8_t maxconn;
};

template <int N>
struct PACKED SystemReadMemory {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 7;
    uint32_t address;
    uint8_t length;
};

template <int N>
struct PACKED SystemReadMemoryResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 7;
    uint32_t address;
    uint8_t data[N];
};

struct PACKED SystemGetInfo {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 8;
};

struct PACKED SystemGetInfoResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 8;
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
    uint16_t build;
    uint16_t ll_version;
    uint8_t protocol_version;
    uint8_t hw;
};

template <int N>
struct PACKED SystemEndpointTx {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 9;
    uint8_t endpoint;
    uint8_t data[N];
};

struct PACKED SystemEndpointTxResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 9;
    uint16_t result;
};

struct PACKED SystemWhitelistAppend {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 10;
    uint8_t address[6];
    uint8_t address_type;
};

struct PACKED SystemWhitelistAppendResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 10;
    uint16_t result;
};

struct PACKED SystemWhitelistRemove {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 11;
    uint8_t address[6];
    uint8_t address_type;
};

struct PACKED SystemWhitelistRemoveResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 11;
    uint16_t result;
};

struct PACKED SystemWhitelistClear {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 12;
};

struct PACKED SystemWhitelistClearResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 12;
};

template <int N>
struct PACKED SystemEndpointRx {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 13;
    uint8_t endpoint;
    uint8_t size;
};

template <int N>
struct PACKED SystemEndpointRxResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 13;
    uint16_t result;
    uint8_t data[N];
};

struct PACKED SystemEndpointSetWatermarks {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 14;
    uint8_t endpoint;
    uint8_t rx;
    uint8_t tx;
};

struct PACKED SystemEndpointSetWatermarksResponse {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 14;
    uint16_t result;
};

struct PACKED SystemBootEvent {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 0;
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
    uint16_t build;
    uint16_t ll_version;
    uint8_t protocol_version;
    uint8_t hw;
};

template <int N>
struct PACKED SystemDebugEvent {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 1;
    uint8_t data[N];
};

struct PACKED SystemEndpointWatermarkRxEvent {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 2;
    uint8_t endpoint;
    uint8_t data;
};

struct PACKED SystemEndpointWatermarkTxEvent {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 3;
    uint8_t endpoint;
    uint8_t data;
};

struct PACKED SystemScriptFailureEvent {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 4;
    uint16_t address;
    uint16_t reason;
};

struct PACKED SystemNoLicenseKeyEvent {
    static constexpr uint8_t cls = 0;
    static constexpr uint8_t cmd = 5;
};

struct PACKED FlashPsDefrag {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 0;
};

struct PACKED FlashPsDefragResponse {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 0;
};

struct PACKED FlashPsDump {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 1;
};

struct PACKED FlashPsDumpResponse {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 1;
};

struct PACKED FlashPsEraseAll {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 2;
};

struct PACKED FlashPsEraseAllResponse {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 2;
};

template <int N>
struct PACKED FlashPsSave {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 3;
    uint16_t key;
    uint8_t value[N];
};

struct PACKED FlashPsSaveResponse {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 3;
    uint16_t result;
};

template <int N>
struct PACKED FlashPsLoad {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 4;
    uint16_t key;
};

template <int N>
struct PACKED FlashPsLoadResponse {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 4;
    uint16_t result;
    uint8_t value[N];
};

struct PACKED FlashPsErase {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 5;
    uint16_t key;
};

struct PACKED FlashPsEraseResponse {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 5;
};

struct PACKED FlashErasePage {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 6;
    uint8_t page;
};

struct PACKED FlashErasePageResponse {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 6;
    uint16_t result;
};

template <int N>
struct PACKED FlashWriteWords {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 7;
    uint16_t address;
    uint8_t words[N];
};

struct PACKED FlashWriteWordsResponse {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 7;
};

template <int N>
struct PACKED FlashPsKeyEvent {
    static constexpr uint8_t cls = 1;
    static constexpr uint8_t cmd = 0;
    uint16_t key;
    uint8_t value[N];
};

template <int N>
struct PACKED AttributesWrite {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 0;
    uint16_t handle;
    uint8_t offset;
    uint8_t value[N];
};

struct PACKED AttributesWriteResponse {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 0;
    uint16_t result;
};

template <int N>
struct PACKED AttributesRead {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 1;
    uint16_t handle;
    uint16_t offset;
};

template <int N>
struct PACKED AttributesReadResponse {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 1;
    uint16_t handle;
    uint16_t offset;
    uint16_t result;
    uint8_t value[N];
};

template <int N>
struct PACKED AttributesReadType {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 2;
    uint16_t handle;
};

template <int N>
struct PACKED AttributesReadTypeResponse {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 2;
    uint16_t handle;
    uint16_t result;
    uint8_t value[N];
};

template <int N>
struct PACKED AttributesUserReadResponse {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 3;
    uint8_t connection;
    uint8_t att_error;
    uint8_t value[N];
};

struct PACKED AttributesUserReadResponseResponse {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 3;
};

struct PACKED AttributesUserWriteResponse {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 4;
    uint8_t connection;
    uint8_t att_error;
};

struct PACKED AttributesUserWriteResponseResponse {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 4;
};

template <int N>
struct PACKED AttributesValueEvent {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 0;
    uint8_t connection;
    uint8_t reason;
    uint16_t handle;
    uint16_t offset;
    uint8_t value[N];
};

struct PACKED AttributesUserReadRequestEvent {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 1;
    uint8_t connection;
    uint16_t handle;
    uint16_t offset;
    uint8_t maxsize;
};

struct PACKED AttributesStatusEvent {
    static constexpr uint8_t cls = 2;
    static constexpr uint8_t cmd = 2;
    uint16_t handle;
    uint8_t flags;
};

struct PACKED ConnectionDisconnect {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 0;
    uint8_t connection;
};

struct PACKED ConnectionDisconnectResponse {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 0;
    uint8_t connection;
    uint16_t result;
};

struct PACKED ConnectionGetRssi {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 1;
    uint8_t connection;
};

struct PACKED ConnectionGetRssiResponse {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 1;
    uint8_t connection;
    int8_t rssi;
};

struct PACKED ConnectionUpdate {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 2;
    uint8_t connection;
    uint16_t interval_min;
    uint16_t interval_max;
    uint16_t latency;
    uint16_t timeout;
};

struct PACKED ConnectionUpdateResponse {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 2;
    uint8_t connection;
    uint16_t result;
};

struct PACKED ConnectionVersionUpdate {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 3;
    uint8_t connection;
};

struct PACKED ConnectionVersionUpdateResponse {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 3;
    uint8_t connection;
    uint16_t result;
};

template <int N>
struct PACKED ConnectionChannelMapGet {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 4;
    uint8_t connection;
};

template <int N>
struct PACKED ConnectionChannelMapGetResponse {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 4;
    uint8_t connection;
    uint8_t map[N];
};

template <int N>
struct PACKED ConnectionChannelMapSet {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 5;
    uint8_t connection;
    uint8_t map[N];
};

struct PACKED ConnectionChannelMapSetResponse {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 5;
    uint8_t connection;
    uint16_t result;
};

struct PACKED ConnectionFeaturesGet {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 6;
    uint8_t connection;
};

struct PACKED ConnectionFeaturesGetResponse {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 6;
    uint8_t connection;
    uint16_t result;
};

struct PACKED ConnectionGetStatus {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 7;
    uint8_t connection;
};

struct PACKED ConnectionGetStatusResponse {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 7;
    uint8_t connection;
};

template <int N>
struct PACKED ConnectionRawTx {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 8;
    uint8_t connection;
    uint8_t data[N];
};

struct PACKED ConnectionRawTxResponse {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 8;
    uint8_t connection;
};

struct PACKED ConnectionStatusEvent {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 0;
    uint8_t connection;
    uint8_t flags;
    uint8_t address[6];
    uint8_t address_type;
    uint16_t conn_interval;
    uint16_t timeout;
    uint16_t latency;
    uint8_t bonding;
};

struct PACKED ConnectionVersionIndEvent {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 1;
    uint8_t connection;
    uint8_t vers_nr;
    uint16_t comp_id;
    uint16_t sub_vers_nr;
};

template <int N>
struct PACKED ConnectionFeatureIndEvent {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 2;
    uint8_t connection;
    uint8_t features[N];
};

template <int N>
struct PACKED ConnectionRawRxEvent {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 3;
    uint8_t connection;
    uint8_t data[N];
};

struct PACKED ConnectionDisconnectedEvent {
    static constexpr uint8_t cls = 3;
    static constexpr uint8_t cmd = 4;
    uint8_t connection;
    uint16_t reason;
};

template <int N>
struct PACKED AttclientFindByTypeValue {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 0;
    uint8_t connection;
    uint16_t start;
    uint16_t end;
    uint16_t uuid;
    uint8_t value[N];
};

struct PACKED AttclientFindByTypeValueResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 0;
    uint8_t connection;
    uint16_t result;
};

template <int N>
struct PACKED AttclientReadByGroupType {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 1;
    uint8_t connection;
    uint16_t start;
    uint16_t end;
    uint8_t uuid[N];
};

struct PACKED AttclientReadByGroupTypeResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 1;
    uint8_t connection;
    uint16_t result;
};

template <int N>
struct PACKED AttclientReadByType {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 2;
    uint8_t connection;
    uint16_t start;
    uint16_t end;
    uint8_t uuid[N];
};

struct PACKED AttclientReadByTypeResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 2;
    uint8_t connection;
    uint16_t result;
};

struct PACKED AttclientFindInformation {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 3;
    uint8_t connection;
    uint16_t start;
    uint16_t end;
};

struct PACKED AttclientFindInformationResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 3;
    uint8_t connection;
    uint16_t result;
};

struct PACKED AttclientReadByHandle {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 4;
    uint8_t connection;
    uint16_t chrhandle;
};

struct PACKED AttclientReadByHandleResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 4;
    uint8_t connection;
    uint16_t result;
};

template <int N>
struct PACKED AttclientAttributeWrite {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 5;
    uint8_t connection;
    uint16_t atthandle;
    uint8_t data[N];
};

struct PACKED AttclientAttributeWriteResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 5;
    uint8_t connection;
    uint16_t result;
};

template <int N>
struct PACKED AttclientWriteCommand {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 6;
    uint8_t connection;
    uint16_t atthandle;
    uint8_t data[N];
};

struct PACKED AttclientWriteCommandResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 6;
    uint8_t connection;
    uint16_t result;
};

struct PACKED AttclientIndicateConfirm {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 7;
    uint8_t connection;
};

struct PACKED AttclientIndicateConfirmResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 7;
    uint16_t result;
};

struct PACKED AttclientReadLong {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 8;
    uint8_t connection;
    uint16_t chrhandle;
};

struct PACKED AttclientReadLongResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 8;
    uint8_t connection;
    uint16_t result;
};

template <int N>
struct PACKED AttclientPrepareWrite {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 9;
    uint8_t connection;
    uint16_t atthandle;
    uint16_t offset;
    uint8_t data[N];
};

struct PACKED AttclientPrepareWriteResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 9;
    uint8_t connection;
    uint16_t result;
};

struct PACKED AttclientExecuteWrite {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 10;
    uint8_t connection;
    uint8_t commit;
};

struct PACKED AttclientExecuteWriteResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 10;
    uint8_t connection;
    uint16_t result;
};

template <int N>
struct PACKED AttclientReadMultiple {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 11;
    uint8_t connection;
    uint8_t handles[N];
};

struct PACKED AttclientReadMultipleResponse {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 11;
    uint8_t connection;
    uint16_t result;
};

struct PACKED AttclientIndicatedEvent {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 0;
    uint8_t connection;
    uint16_t attrhandle;
};

struct PACKED AttclientProcedureCompletedEvent {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 1;
    uint8_t connection;
    uint16_t result;
    uint16_t chrhandle;
};

template <int N>
struct PACKED AttclientGroupFoundEvent {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 2;
    uint8_t connection;
    uint16_t start;
    uint16_t end;
    uint8_t uuid[N];
};

template <int N>
struct PACKED AttclientAttributeFoundEvent {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 3;
    uint8_t connection;
    uint16_t chrdecl;
    uint16_t value;
    uint8_t properties;
    uint8_t uuid[N];
};

template <int N>
struct PACKED AttclientFindInformationFoundEvent {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 4;
    uint8_t connection;
    uint16_t chrhandle;
    uint8_t uuid[N];
};

template <int N>
struct PACKED AttclientAttributeValueEvent {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 5;
    uint8_t connection;
    uint16_t atthandle;
    uint8_t type;
    uint8_t value[N];
};

template <int N>
struct PACKED AttclientReadMultipleResponseEvent {
    static constexpr uint8_t cls = 4;
    static constexpr uint8_t cmd = 6;
    uint8_t connection;
    uint8_t handles[N];
};

struct PACKED SmEncryptStart {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 0;
    uint8_t handle;
    uint8_t bonding;
};

struct PACKED SmEncryptStartResponse {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 0;
    uint8_t handle;
    uint16_t result;
};

struct PACKED SmSetBondableMode {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 1;
    uint8_t bondable;
};

struct PACKED SmSetBondableModeResponse {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 1;
};

struct PACKED SmDeleteBonding {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 2;
    uint8_t handle;
};

struct PACKED SmDeleteBondingResponse {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 2;
    uint16_t result;
};

struct PACKED SmSetParameters {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 3;
    uint8_t mitm;
    uint8_t min_key_size;
    uint8_t io_capabilities;
};

struct PACKED SmSetParametersResponse {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 3;
};

struct PACKED SmPasskeyEntry {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 4;
    uint8_t handle;
    uint32_t passkey;
};

struct PACKED SmPasskeyEntryResponse {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 4;
    uint16_t result;
};

struct PACKED SmGetBonds {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 5;
};

struct PACKED SmGetBondsResponse {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 5;
    uint8_t bonds;
};

template <int N>
struct PACKED SmSetOobData {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 6;
    uint8_t oob[N];
};

struct PACKED SmSetOobDataResponse {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 6;
};

template <int N>
struct PACKED SmSmpDataEvent {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 0;
    uint8_t handle;
    uint8_t packet;
    uint8_t data[N];
};

struct PACKED SmBondingFailEvent {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 1;
    uint8_t handle;
    uint16_t result;
};

struct PACKED SmPasskeyDisplayEvent {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 2;
    uint8_t handle;
    uint32_t passkey;
};

struct PACKED SmPasskeyRequestEvent {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 3;
    uint8_t handle;
};

struct PACKED SmBondStatusEvent {
    static constexpr uint8_t cls = 5;
    static constexpr uint8_t cmd = 4;
    uint8_t bond;
    uint8_t keysize;
    uint8_t mitm;
    uint8_t keys;
};

struct PACKED GapSetPrivacyFlags {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 0;
    uint8_t peripheral_privacy;
    uint8_t central_privacy;
};

struct PACKED GapSetPrivacyFlagsResponse {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 0;
};

struct PACKED GapSetMode {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 1;
    uint8_t discover;
    uint8_t connect;
};

struct PACKED GapSetModeResponse {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 1;
    uint16_t result;
};

struct PACKED GapDiscover {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 2;
    uint8_t mode;
};

struct PACKED GapDiscoverResponse {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 2;
    uint16_t result;
};

struct PACKED GapConnectDirect {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 3;
    uint8_t address[6];
    uint8_t addr_type;
    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t timeout;
    uint16_t latency;
};

struct PACKED GapConnectDirectResponse {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 3;
    uint16_t result;
    uint8_t connection_handle;
};

struct PACKED GapEndProcedure {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 4;
};

struct PACKED GapEndProcedureResponse {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 4;
    uint16_t result;
};

struct PACKED GapConnectSelective {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 5;
    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t timeout;
    uint16_t latency;
};

struct PACKED GapConnectSelectiveResponse {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 5;
    uint16_t result;
    uint8_t connection_handle;
};

struct PACKED GapSetFiltering {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 6;
    uint8_t scan_policy;
    uint8_t adv_policy;
    uint8_t scan_duplicate_filtering;
};

struct PACKED GapSetFilteringResponse {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 6;
    uint16_t result;
};

struct PACKED GapSetScanParameters {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 7;
    uint16_t scan_interval;
    uint16_t scan_window;
    uint8_t active;
};

struct PACKED GapSetScanParametersResponse {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 7;
    uint16_t result;
};

struct PACKED GapSetAdvParameters {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 8;
    uint16_t adv_interval_min;
    uint16_t adv_interval_max;
    uint8_t adv_channels;
};

struct PACKED GapSetAdvParametersResponse {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 8;
    uint16_t result;
};

template <int N>
struct PACKED GapSetAdvData {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 9;
    uint8_t set_scanrsp;
    uint8_t adv_data[N];
};

struct PACKED GapSetAdvDataResponse {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 9;
    uint16_t result;
};

struct PACKED GapSetDirectedConnectableMode {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 10;
    uint8_t address[6];
    uint8_t addr_type;
};

struct PACKED GapSetDirectedConnectableModeResponse {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 10;
    uint16_t result;
};

template <int N>
struct PACKED GapScanResponseEvent {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 0;
    int8_t rssi;
    uint8_t packet_type;
    uint8_t sender[6];
    uint8_t address_type;
    uint8_t bond;
    uint8_t data[N];
};

struct PACKED GapModeChangedEvent {
    static constexpr uint8_t cls = 6;
    static constexpr uint8_t cmd = 1;
    uint8_t discover;
    uint8_t connect;
};

struct PACKED HardwareIoPortConfigIrq {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 0;
    uint8_t port;
    uint8_t enable_bits;
    uint8_t falling_edge;
};

struct PACKED HardwareIoPortConfigIrqResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 0;
    uint16_t result;
};

struct PACKED HardwareSetSoftTimer {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 1;
    uint32_t time;
    uint8_t handle;
    uint8_t single_shot;
};

struct PACKED HardwareSetSoftTimerResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 1;
    uint16_t result;
};

struct PACKED HardwareAdcRead {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 2;
    uint8_t input;
    uint8_t decimation;
    uint8_t reference_selection;
};

struct PACKED HardwareAdcReadResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 2;
    uint16_t result;
};

struct PACKED HardwareIoPortConfigDirection {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 3;
    uint8_t port;
    uint8_t direction;
};

struct PACKED HardwareIoPortConfigDirectionResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 3;
    uint16_t result;
};

struct PACKED HardwareIoPortConfigFunction {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 4;
    uint8_t port;
    uint8_t function;
};

struct PACKED HardwareIoPortConfigFunctionResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 4;
    uint16_t result;
};

struct PACKED HardwareIoPortConfigPull {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 5;
    uint8_t port;
    uint8_t tristate_mask;
    uint8_t pull_up;
};

struct PACKED HardwareIoPortConfigPullResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 5;
    uint16_t result;
};

struct PACKED HardwareIoPortWrite {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 6;
    uint8_t port;
    uint8_t mask;
    uint8_t data;
};

struct PACKED HardwareIoPortWriteResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 6;
    uint16_t result;
};

struct PACKED HardwareIoPortRead {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 7;
    uint8_t port;
    uint8_t mask;
};

struct PACKED HardwareIoPortReadResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 7;
    uint16_t result;
    uint8_t port;
    uint8_t data;
};

struct PACKED HardwareSpiConfig {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 8;
    uint8_t channel;
    uint8_t polarity;
    uint8_t phase;
    uint8_t bit_order;
    uint8_t baud_e;
    uint8_t baud_m;
};

struct PACKED HardwareSpiConfigResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 8;
    uint16_t result;
};

template <int N>
struct PACKED HardwareSpiTransfer {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 9;
    uint8_t channel;
    uint8_t data[N];
};

template <int N>
struct PACKED HardwareSpiTransferResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 9;
    uint16_t result;
    uint8_t channel;
    uint8_t data[N];
};

template <int N>
struct PACKED HardwareI2CRead {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 10;
    uint8_t address;
    uint8_t stop;
    uint8_t length;
};

template <int N>
struct PACKED HardwareI2CReadResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 10;
    uint16_t result;
    uint8_t data[N];
};

template <int N>
struct PACKED HardwareI2CWrite {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 11;
    uint8_t address;
    uint8_t stop;
    uint8_t data[N];
};

struct PACKED HardwareI2CWriteResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 11;
    uint8_t written;
};

struct PACKED HardwareSetTxpower {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 12;
    uint8_t power;
};

struct PACKED HardwareSetTxpowerResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 12;
};

struct PACKED HardwareTimerComparator {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 13;
    uint8_t timer;
    uint8_t channel;
    uint8_t mode;
    uint16_t comparator_value;
};

struct PACKED HardwareTimerComparatorResponse {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 13;
    uint16_t result;
};

struct PACKED HardwareIoPortStatusEvent {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 0;
    uint32_t timestamp;
    uint8_t port;
    uint8_t irq;
    uint8_t state;
};

struct PACKED HardwareSoftTimerEvent {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 1;
    uint8_t handle;
};

struct PACKED HardwareAdcResultEvent {
    static constexpr uint8_t cls = 7;
    static constexpr uint8_t cmd = 2;
    uint8_t input;
    int16_t value;
};

struct PACKED TestPhyTx {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 0;
    uint8_t channel;
    uint8_t length;
    uint8_t type;
};

struct PACKED TestPhyTxResponse {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 0;
};

struct PACKED TestPhyRx {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 1;
    uint8_t channel;
};

struct PACKED TestPhyRxResponse {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 1;
};

struct PACKED TestPhyEnd {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 2;
};

struct PACKED TestPhyEndResponse {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 2;
    uint16_t counter;
};

struct PACKED TestPhyReset {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 3;
};

struct PACKED TestPhyResetResponse {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 3;
};

template <int N>
struct PACKED TestGetChannelMap {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 4;
};

template <int N>
struct PACKED TestGetChannelMapResponse {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 4;
    uint8_t channel_map[N];
};

template <int N>
struct PACKED TestDebug {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 5;
    uint8_t input[N];
};

template <int N>
struct PACKED TestDebugResponse {
    static constexpr uint8_t cls = 8;
    static constexpr uint8_t cmd = 5;
    uint8_t output[N];
};

template <typename T>
struct PACKED Command {
    Header header;
    T payload;
};

#endif // BLEAPI_H
