/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef BLEAPI_H
#define BLEAPI_H

#include <cinttypes>
#define PACKED __attribute__((__packed__))

struct PACKED Header {
    std::uint8_t length1 : 3;
    std::uint8_t tech : 4;
    std::uint8_t type : 1;
    std::uint8_t length0;
    std::uint8_t cls;
    std::uint8_t cmd;

    std::size_t length() const
    {
        return static_cast<std::size_t>(length1) << 8 | length0;
    }
};

struct PACKED SystemReset {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t boot_in_dfu;
};

struct PACKED SystemResetResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 0;
};

struct PACKED SystemHello {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 1;
};

struct PACKED SystemHelloResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 1;
};

struct PACKED SystemAddressGet {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 2;
};

struct PACKED SystemAddressGetResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t address[6];
};

struct PACKED SystemRegWrite {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 3;
    std::uint16_t address;
    std::uint8_t value;
};

struct PACKED SystemRegWriteResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 3;
    std::uint16_t result;
};

struct PACKED SystemRegRead {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 4;
    std::uint16_t address;
};

struct PACKED SystemRegReadResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 4;
    std::uint16_t address;
    std::uint8_t value;
};

struct PACKED SystemGetCounters {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 5;
};

struct PACKED SystemGetCountersResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 5;
    std::uint8_t txok;
    std::uint8_t txretry;
    std::uint8_t rxok;
    std::uint8_t rxfail;
    std::uint8_t mbuf;
};

struct PACKED SystemGetConnections {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 6;
};

struct PACKED SystemGetConnectionsResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 6;
    std::uint8_t maxconn;
};

template <int N>
struct PACKED SystemReadMemory {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 7;
    std::uint32_t address;
    std::uint8_t length;
};

template <int N>
struct PACKED SystemReadMemoryResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 7;
    std::uint32_t address;
    std::uint8_t data[N];
};

struct PACKED SystemGetInfo {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 8;
};

struct PACKED SystemGetInfoResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 8;
    std::uint16_t major;
    std::uint16_t minor;
    std::uint16_t patch;
    std::uint16_t build;
    std::uint16_t ll_version;
    std::uint8_t protocol_version;
    std::uint8_t hw;
};

template <int N>
struct PACKED SystemEndpointTx {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 9;
    std::uint8_t endpoint;
    std::uint8_t data[N];
};

struct PACKED SystemEndpointTxResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 9;
    std::uint16_t result;
};

struct PACKED SystemWhitelistAppend {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 10;
    std::uint8_t address[6];
    std::uint8_t address_type;
};

struct PACKED SystemWhitelistAppendResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 10;
    std::uint16_t result;
};

struct PACKED SystemWhitelistRemove {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 11;
    std::uint8_t address[6];
    std::uint8_t address_type;
};

struct PACKED SystemWhitelistRemoveResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 11;
    std::uint16_t result;
};

struct PACKED SystemWhitelistClear {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 12;
};

struct PACKED SystemWhitelistClearResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 12;
};

template <int N>
struct PACKED SystemEndpointRx {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 13;
    std::uint8_t endpoint;
    std::uint8_t size;
};

template <int N>
struct PACKED SystemEndpointRxResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 13;
    std::uint16_t result;
    std::uint8_t data[N];
};

struct PACKED SystemEndpointSetWatermarks {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 14;
    std::uint8_t endpoint;
    std::uint8_t rx;
    std::uint8_t tx;
};

struct PACKED SystemEndpointSetWatermarksResponse {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 14;
    std::uint16_t result;
};

struct PACKED SystemBootEvent {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 0;
    std::uint16_t major;
    std::uint16_t minor;
    std::uint16_t patch;
    std::uint16_t build;
    std::uint16_t ll_version;
    std::uint8_t protocol_version;
    std::uint8_t hw;
};

template <int N>
struct PACKED SystemDebugEvent {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t data[N];
};

struct PACKED SystemEndpointWatermarkRxEvent {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t endpoint;
    std::uint8_t data;
};

struct PACKED SystemEndpointWatermarkTxEvent {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t endpoint;
    std::uint8_t data;
};

struct PACKED SystemScriptFailureEvent {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 4;
    std::uint16_t address;
    std::uint16_t reason;
};

struct PACKED SystemNoLicenseKeyEvent {
    static constexpr std::uint8_t cls = 0;
    static constexpr std::uint8_t cmd = 5;
};

struct PACKED FlashPsDefrag {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 0;
};

struct PACKED FlashPsDefragResponse {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 0;
};

struct PACKED FlashPsDump {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 1;
};

struct PACKED FlashPsDumpResponse {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 1;
};

struct PACKED FlashPsEraseAll {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 2;
};

struct PACKED FlashPsEraseAllResponse {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 2;
};

template <int N>
struct PACKED FlashPsSave {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 3;
    std::uint16_t key;
    std::uint8_t value[N];
};

struct PACKED FlashPsSaveResponse {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 3;
    std::uint16_t result;
};

template <int N>
struct PACKED FlashPsLoad {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 4;
    std::uint16_t key;
};

template <int N>
struct PACKED FlashPsLoadResponse {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 4;
    std::uint16_t result;
    std::uint8_t value[N];
};

struct PACKED FlashPsErase {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 5;
    std::uint16_t key;
};

struct PACKED FlashPsEraseResponse {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 5;
};

struct PACKED FlashErasePage {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 6;
    std::uint8_t page;
};

struct PACKED FlashErasePageResponse {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 6;
    std::uint16_t result;
};

template <int N>
struct PACKED FlashWriteWords {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 7;
    std::uint16_t address;
    std::uint8_t words[N];
};

struct PACKED FlashWriteWordsResponse {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 7;
};

template <int N>
struct PACKED FlashPsKeyEvent {
    static constexpr std::uint8_t cls = 1;
    static constexpr std::uint8_t cmd = 0;
    std::uint16_t key;
    std::uint8_t value[N];
};

template <int N>
struct PACKED AttributesWrite {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 0;
    std::uint16_t handle;
    std::uint8_t offset;
    std::uint8_t value[N];
};

struct PACKED AttributesWriteResponse {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 0;
    std::uint16_t result;
};

template <int N>
struct PACKED AttributesRead {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 1;
    std::uint16_t handle;
    std::uint16_t offset;
};

template <int N>
struct PACKED AttributesReadResponse {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 1;
    std::uint16_t handle;
    std::uint16_t offset;
    std::uint16_t result;
    std::uint8_t value[N];
};

template <int N>
struct PACKED AttributesReadType {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 2;
    std::uint16_t handle;
};

template <int N>
struct PACKED AttributesReadTypeResponse {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 2;
    std::uint16_t handle;
    std::uint16_t result;
    std::uint8_t value[N];
};

template <int N>
struct PACKED AttributesUserReadResponse {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t connection;
    std::uint8_t att_error;
    std::uint8_t value[N];
};

struct PACKED AttributesUserReadResponseResponse {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 3;
};

struct PACKED AttributesUserWriteResponse {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 4;
    std::uint8_t connection;
    std::uint8_t att_error;
};

struct PACKED AttributesUserWriteResponseResponse {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 4;
};

template <int N>
struct PACKED AttributesValueEvent {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t connection;
    std::uint8_t reason;
    std::uint16_t handle;
    std::uint16_t offset;
    std::uint8_t value[N];
};

struct PACKED AttributesUserReadRequestEvent {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t connection;
    std::uint16_t handle;
    std::uint16_t offset;
    std::uint8_t maxsize;
};

struct PACKED AttributesStatusEvent {
    static constexpr std::uint8_t cls = 2;
    static constexpr std::uint8_t cmd = 2;
    std::uint16_t handle;
    std::uint8_t flags;
};

struct PACKED ConnectionDisconnect {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t connection;
};

struct PACKED ConnectionDisconnectResponse {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED ConnectionGetRssi {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t connection;
};

struct PACKED ConnectionGetRssiResponse {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t connection;
    std::int8_t rssi;
};

struct PACKED ConnectionUpdate {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t connection;
    std::uint16_t interval_min;
    std::uint16_t interval_max;
    std::uint16_t latency;
    std::uint16_t timeout;
};

struct PACKED ConnectionUpdateResponse {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED ConnectionVersionUpdate {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t connection;
};

struct PACKED ConnectionVersionUpdateResponse {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED ConnectionChannelMapGet {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 4;
    std::uint8_t connection;
};

template <int N>
struct PACKED ConnectionChannelMapGetResponse {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 4;
    std::uint8_t connection;
    std::uint8_t map[N];
};

template <int N>
struct PACKED ConnectionChannelMapSet {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 5;
    std::uint8_t connection;
    std::uint8_t map[N];
};

struct PACKED ConnectionChannelMapSetResponse {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 5;
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED ConnectionFeaturesGet {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 6;
    std::uint8_t connection;
};

struct PACKED ConnectionFeaturesGetResponse {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 6;
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED ConnectionGetStatus {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 7;
    std::uint8_t connection;
};

struct PACKED ConnectionGetStatusResponse {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 7;
    std::uint8_t connection;
};

template <int N>
struct PACKED ConnectionRawTx {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 8;
    std::uint8_t connection;
    std::uint8_t data[N];
};

struct PACKED ConnectionRawTxResponse {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 8;
    std::uint8_t connection;
};

struct PACKED ConnectionStatusEvent {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t connection;
    std::uint8_t flags;
    std::uint8_t address[6];
    std::uint8_t address_type;
    std::uint16_t conn_interval;
    std::uint16_t timeout;
    std::uint16_t latency;
    std::uint8_t bonding;
};

struct PACKED ConnectionVersionIndEvent {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t connection;
    std::uint8_t vers_nr;
    std::uint16_t comp_id;
    std::uint16_t sub_vers_nr;
};

template <int N>
struct PACKED ConnectionFeatureIndEvent {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t connection;
    std::uint8_t features[N];
};

template <int N>
struct PACKED ConnectionRawRxEvent {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t connection;
    std::uint8_t data[N];
};

struct PACKED ConnectionDisconnectedEvent {
    static constexpr std::uint8_t cls = 3;
    static constexpr std::uint8_t cmd = 4;
    std::uint8_t connection;
    std::uint16_t reason;
};

template <int N>
struct PACKED AttclientFindByTypeValue {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint16_t uuid;
    std::uint8_t value[N];
};

struct PACKED AttclientFindByTypeValueResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientReadByGroupType {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint8_t uuid[N];
};

struct PACKED AttclientReadByGroupTypeResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientReadByType {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint8_t uuid[N];
};

struct PACKED AttclientReadByTypeResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED AttclientFindInformation {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
};

struct PACKED AttclientFindInformationResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED AttclientReadByHandle {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 4;
    std::uint8_t connection;
    std::uint16_t chrhandle;
};

struct PACKED AttclientReadByHandleResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 4;
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientAttributeWrite {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 5;
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint8_t data[N];
};

struct PACKED AttclientAttributeWriteResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 5;
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientWriteCommand {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 6;
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint8_t data[N];
};

struct PACKED AttclientWriteCommandResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 6;
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED AttclientIndicateConfirm {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 7;
    std::uint8_t connection;
};

struct PACKED AttclientIndicateConfirmResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 7;
    std::uint16_t result;
};

struct PACKED AttclientReadLong {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 8;
    std::uint8_t connection;
    std::uint16_t chrhandle;
};

struct PACKED AttclientReadLongResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 8;
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientPrepareWrite {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 9;
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint16_t offset;
    std::uint8_t data[N];
};

struct PACKED AttclientPrepareWriteResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 9;
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED AttclientExecuteWrite {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 10;
    std::uint8_t connection;
    std::uint8_t commit;
};

struct PACKED AttclientExecuteWriteResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 10;
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientReadMultiple {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 11;
    std::uint8_t connection;
    std::uint8_t handles[N];
};

struct PACKED AttclientReadMultipleResponse {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 11;
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED AttclientIndicatedEvent {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t connection;
    std::uint16_t attrhandle;
};

struct PACKED AttclientProcedureCompletedEvent {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t connection;
    std::uint16_t result;
    std::uint16_t chrhandle;
};

template <int N>
struct PACKED AttclientGroupFoundEvent {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint8_t uuid[N];
};

template <int N>
struct PACKED AttclientAttributeFoundEvent {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t connection;
    std::uint16_t chrdecl;
    std::uint16_t value;
    std::uint8_t properties;
    std::uint8_t uuid[N];
};

template <int N>
struct PACKED AttclientFindInformationFoundEvent {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 4;
    std::uint8_t connection;
    std::uint16_t chrhandle;
    std::uint8_t uuid[N];
};

template <int N>
struct PACKED AttclientAttributeValueEvent {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 5;
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint8_t type;
    std::uint8_t value[N];
};

template <int N>
struct PACKED AttclientReadMultipleResponseEvent {
    static constexpr std::uint8_t cls = 4;
    static constexpr std::uint8_t cmd = 6;
    std::uint8_t connection;
    std::uint8_t handles[N];
};

struct PACKED SmEncryptStart {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t handle;
    std::uint8_t bonding;
};

struct PACKED SmEncryptStartResponse {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t handle;
    std::uint16_t result;
};

struct PACKED SmSetBondableMode {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t bondable;
};

struct PACKED SmSetBondableModeResponse {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 1;
};

struct PACKED SmDeleteBonding {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t handle;
};

struct PACKED SmDeleteBondingResponse {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 2;
    std::uint16_t result;
};

struct PACKED SmSetParameters {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t mitm;
    std::uint8_t min_key_size;
    std::uint8_t io_capabilities;
};

struct PACKED SmSetParametersResponse {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 3;
};

struct PACKED SmPasskeyEntry {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 4;
    std::uint8_t handle;
    std::uint32_t passkey;
};

struct PACKED SmPasskeyEntryResponse {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 4;
    std::uint16_t result;
};

struct PACKED SmGetBonds {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 5;
};

struct PACKED SmGetBondsResponse {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 5;
    std::uint8_t bonds;
};

template <int N>
struct PACKED SmSetOobData {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 6;
    std::uint8_t oob[N];
};

struct PACKED SmSetOobDataResponse {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 6;
};

template <int N>
struct PACKED SmSmpDataEvent {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t handle;
    std::uint8_t packet;
    std::uint8_t data[N];
};

struct PACKED SmBondingFailEvent {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t handle;
    std::uint16_t result;
};

struct PACKED SmPasskeyDisplayEvent {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t handle;
    std::uint32_t passkey;
};

struct PACKED SmPasskeyRequestEvent {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t handle;
};

struct PACKED SmBondStatusEvent {
    static constexpr std::uint8_t cls = 5;
    static constexpr std::uint8_t cmd = 4;
    std::uint8_t bond;
    std::uint8_t keysize;
    std::uint8_t mitm;
    std::uint8_t keys;
};

struct PACKED GapSetPrivacyFlags {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t peripheral_privacy;
    std::uint8_t central_privacy;
};

struct PACKED GapSetPrivacyFlagsResponse {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 0;
};

struct PACKED GapSetMode {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t discover;
    std::uint8_t connect;
};

struct PACKED GapSetModeResponse {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 1;
    std::uint16_t result;
};

struct PACKED GapDiscover {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t mode;
};

struct PACKED GapDiscoverResponse {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 2;
    std::uint16_t result;
};

struct PACKED GapConnectDirect {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t address[6];
    std::uint8_t addr_type;
    std::uint16_t conn_interval_min;
    std::uint16_t conn_interval_max;
    std::uint16_t timeout;
    std::uint16_t latency;
};

struct PACKED GapConnectDirectResponse {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 3;
    std::uint16_t result;
    std::uint8_t connection_handle;
};

struct PACKED GapEndProcedure {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 4;
};

struct PACKED GapEndProcedureResponse {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 4;
    std::uint16_t result;
};

struct PACKED GapConnectSelective {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 5;
    std::uint16_t conn_interval_min;
    std::uint16_t conn_interval_max;
    std::uint16_t timeout;
    std::uint16_t latency;
};

struct PACKED GapConnectSelectiveResponse {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 5;
    std::uint16_t result;
    std::uint8_t connection_handle;
};

struct PACKED GapSetFiltering {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 6;
    std::uint8_t scan_policy;
    std::uint8_t adv_policy;
    std::uint8_t scan_duplicate_filtering;
};

struct PACKED GapSetFilteringResponse {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 6;
    std::uint16_t result;
};

struct PACKED GapSetScanParameters {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 7;
    std::uint16_t scan_interval;
    std::uint16_t scan_window;
    std::uint8_t active;
};

struct PACKED GapSetScanParametersResponse {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 7;
    std::uint16_t result;
};

struct PACKED GapSetAdvParameters {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 8;
    std::uint16_t adv_interval_min;
    std::uint16_t adv_interval_max;
    std::uint8_t adv_channels;
};

struct PACKED GapSetAdvParametersResponse {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 8;
    std::uint16_t result;
};

template <int N>
struct PACKED GapSetAdvData {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 9;
    std::uint8_t set_scanrsp;
    std::uint8_t adv_data[N];
};

struct PACKED GapSetAdvDataResponse {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 9;
    std::uint16_t result;
};

struct PACKED GapSetDirectedConnectableMode {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 10;
    std::uint8_t address[6];
    std::uint8_t addr_type;
};

struct PACKED GapSetDirectedConnectableModeResponse {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 10;
    std::uint16_t result;
};

template <int N>
struct PACKED GapScanResponseEvent {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 0;
    std::int8_t rssi;
    std::uint8_t packet_type;
    std::uint8_t sender[6];
    std::uint8_t address_type;
    std::uint8_t bond;
    std::uint8_t data[N];
};

struct PACKED GapModeChangedEvent {
    static constexpr std::uint8_t cls = 6;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t discover;
    std::uint8_t connect;
};

struct PACKED HardwareIoPortConfigIrq {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t port;
    std::uint8_t enable_bits;
    std::uint8_t falling_edge;
};

struct PACKED HardwareIoPortConfigIrqResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 0;
    std::uint16_t result;
};

struct PACKED HardwareSetSoftTimer {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 1;
    std::uint32_t time;
    std::uint8_t handle;
    std::uint8_t single_shot;
};

struct PACKED HardwareSetSoftTimerResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 1;
    std::uint16_t result;
};

struct PACKED HardwareAdcRead {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t input;
    std::uint8_t decimation;
    std::uint8_t reference_selection;
};

struct PACKED HardwareAdcReadResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 2;
    std::uint16_t result;
};

struct PACKED HardwareIoPortConfigDirection {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 3;
    std::uint8_t port;
    std::uint8_t direction;
};

struct PACKED HardwareIoPortConfigDirectionResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 3;
    std::uint16_t result;
};

struct PACKED HardwareIoPortConfigFunction {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 4;
    std::uint8_t port;
    std::uint8_t function;
};

struct PACKED HardwareIoPortConfigFunctionResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 4;
    std::uint16_t result;
};

struct PACKED HardwareIoPortConfigPull {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 5;
    std::uint8_t port;
    std::uint8_t tristate_mask;
    std::uint8_t pull_up;
};

struct PACKED HardwareIoPortConfigPullResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 5;
    std::uint16_t result;
};

struct PACKED HardwareIoPortWrite {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 6;
    std::uint8_t port;
    std::uint8_t mask;
    std::uint8_t data;
};

struct PACKED HardwareIoPortWriteResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 6;
    std::uint16_t result;
};

struct PACKED HardwareIoPortRead {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 7;
    std::uint8_t port;
    std::uint8_t mask;
};

struct PACKED HardwareIoPortReadResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 7;
    std::uint16_t result;
    std::uint8_t port;
    std::uint8_t data;
};

struct PACKED HardwareSpiConfig {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 8;
    std::uint8_t channel;
    std::uint8_t polarity;
    std::uint8_t phase;
    std::uint8_t bit_order;
    std::uint8_t baud_e;
    std::uint8_t baud_m;
};

struct PACKED HardwareSpiConfigResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 8;
    std::uint16_t result;
};

template <int N>
struct PACKED HardwareSpiTransfer {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 9;
    std::uint8_t channel;
    std::uint8_t data[N];
};

template <int N>
struct PACKED HardwareSpiTransferResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 9;
    std::uint16_t result;
    std::uint8_t channel;
    std::uint8_t data[N];
};

template <int N>
struct PACKED HardwareI2CRead {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 10;
    std::uint8_t address;
    std::uint8_t stop;
    std::uint8_t length;
};

template <int N>
struct PACKED HardwareI2CReadResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 10;
    std::uint16_t result;
    std::uint8_t data[N];
};

template <int N>
struct PACKED HardwareI2CWrite {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 11;
    std::uint8_t address;
    std::uint8_t stop;
    std::uint8_t data[N];
};

struct PACKED HardwareI2CWriteResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 11;
    std::uint8_t written;
};

struct PACKED HardwareSetTxpower {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 12;
    std::uint8_t power;
};

struct PACKED HardwareSetTxpowerResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 12;
};

struct PACKED HardwareTimerComparator {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 13;
    std::uint8_t timer;
    std::uint8_t channel;
    std::uint8_t mode;
    std::uint16_t comparator_value;
};

struct PACKED HardwareTimerComparatorResponse {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 13;
    std::uint16_t result;
};

struct PACKED HardwareIoPortStatusEvent {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 0;
    std::uint32_t timestamp;
    std::uint8_t port;
    std::uint8_t irq;
    std::uint8_t state;
};

struct PACKED HardwareSoftTimerEvent {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t handle;
};

struct PACKED HardwareAdcResultEvent {
    static constexpr std::uint8_t cls = 7;
    static constexpr std::uint8_t cmd = 2;
    std::uint8_t input;
    std::int16_t value;
};

struct PACKED TestPhyTx {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 0;
    std::uint8_t channel;
    std::uint8_t length;
    std::uint8_t type;
};

struct PACKED TestPhyTxResponse {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 0;
};

struct PACKED TestPhyRx {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 1;
    std::uint8_t channel;
};

struct PACKED TestPhyRxResponse {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 1;
};

struct PACKED TestPhyEnd {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 2;
};

struct PACKED TestPhyEndResponse {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 2;
    std::uint16_t counter;
};

struct PACKED TestPhyReset {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 3;
};

struct PACKED TestPhyResetResponse {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 3;
};

template <int N>
struct PACKED TestGetChannelMap {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 4;
};

template <int N>
struct PACKED TestGetChannelMapResponse {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 4;
    std::uint8_t channel_map[N];
};

template <int N>
struct PACKED TestDebug {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 5;
    std::uint8_t input[N];
};

template <int N>
struct PACKED TestDebugResponse {
    static constexpr std::uint8_t cls = 8;
    static constexpr std::uint8_t cmd = 5;
    std::uint8_t output[N];
};

template <typename T>
struct PACKED Command {
    Header header;
    T payload;
};

#endif // BLEAPI_H
