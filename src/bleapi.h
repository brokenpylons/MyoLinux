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

template <typename T>
Header getHeader(std::size_t data_size = 0)
{
    const auto size = data_size + sizeof(T);
    return Header{static_cast<std::uint8_t>(size >> 8), 0, 0,
                static_cast<std::uint8_t>(size & 0xFF), T::cls, T::cmd};
}

template <typename T>
struct Partial {
    enum { value = false };
};

template <template<int> class T, int N>
struct Partial<T<N>> {
    enum { value = false };
};

template <template<int> class T>
struct Partial<T<0>> {
    enum { value = true };
};

struct PACKED SystemReset {
    enum { cls = 0, cmd = 0 };
    std::uint8_t boot_in_dfu;
};

struct PACKED SystemResetResponse {
    enum { cls = 0, cmd = 0 };
};

struct PACKED SystemHello {
    enum { cls = 0, cmd = 1 };
};

struct PACKED SystemHelloResponse {
    enum { cls = 0, cmd = 1 };
};

struct PACKED SystemAddressGet {
    enum { cls = 0, cmd = 2 };
};

struct PACKED SystemAddressGetResponse {
    enum { cls = 0, cmd = 2 };
    std::uint8_t address[6];
};

struct PACKED SystemRegWrite {
    enum { cls = 0, cmd = 3 };
    std::uint16_t address;
    std::uint8_t value;
};

struct PACKED SystemRegWriteResponse {
    enum { cls = 0, cmd = 3 };
    std::uint16_t result;
};

struct PACKED SystemRegRead {
    enum { cls = 0, cmd = 4 };
    std::uint16_t address;
};

struct PACKED SystemRegReadResponse {
    enum { cls = 0, cmd = 4 };
    std::uint16_t address;
    std::uint8_t value;
};

struct PACKED SystemGetCounters {
    enum { cls = 0, cmd = 5 };
};

struct PACKED SystemGetCountersResponse {
    enum { cls = 0, cmd = 5 };
    std::uint8_t txok;
    std::uint8_t txretry;
    std::uint8_t rxok;
    std::uint8_t rxfail;
    std::uint8_t mbuf;
};

struct PACKED SystemGetConnections {
    enum { cls = 0, cmd = 6 };
};

struct PACKED SystemGetConnectionsResponse {
    enum { cls = 0, cmd = 6 };
    std::uint8_t maxconn;
};

struct PACKED SystemReadMemory {
    enum { cls = 0, cmd = 7 };
    std::uint32_t address;
    std::uint8_t length;
};

template <int N>
struct PACKED SystemReadMemoryResponse {
    enum { cls = 0, cmd = 7 };
    std::uint32_t address;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED SystemReadMemoryResponse<0> {
    enum { cls = 0, cmd = 7 };
    std::uint32_t address;
    std::uint8_t length;
};

struct PACKED SystemGetInfo {
    enum { cls = 0, cmd = 8 };
};

struct PACKED SystemGetInfoResponse {
    enum { cls = 0, cmd = 8 };
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
    enum { cls = 0, cmd = 9 };
    std::uint8_t endpoint;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED SystemEndpointTx<0> {
    enum { cls = 0, cmd = 9 };
    std::uint8_t endpoint;
    std::uint8_t length;
};

struct PACKED SystemEndpointTxResponse {
    enum { cls = 0, cmd = 9 };
    std::uint16_t result;
};

struct PACKED SystemWhitelistAppend {
    enum { cls = 0, cmd = 10 };
    std::uint8_t address[6];
    std::uint8_t address_type;
};

struct PACKED SystemWhitelistAppendResponse {
    enum { cls = 0, cmd = 10 };
    std::uint16_t result;
};

struct PACKED SystemWhitelistRemove {
    enum { cls = 0, cmd = 11 };
    std::uint8_t address[6];
    std::uint8_t address_type;
};

struct PACKED SystemWhitelistRemoveResponse {
    enum { cls = 0, cmd = 11 };
    std::uint16_t result;
};

struct PACKED SystemWhitelistClear {
    enum { cls = 0, cmd = 12 };
};

struct PACKED SystemWhitelistClearResponse {
    enum { cls = 0, cmd = 12 };
};

struct PACKED SystemEndpointRx {
    enum { cls = 0, cmd = 13 };
    std::uint8_t endpoint;
    std::uint8_t size;
};

template <int N>
struct PACKED SystemEndpointRxResponse {
    enum { cls = 0, cmd = 13 };
    std::uint16_t result;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED SystemEndpointRxResponse<0> {
    enum { cls = 0, cmd = 13 };
    std::uint16_t result;
    std::uint8_t length;
};

struct PACKED SystemEndpointSetWatermarks {
    enum { cls = 0, cmd = 14 };
    std::uint8_t endpoint;
    std::uint8_t rx;
    std::uint8_t tx;
};

struct PACKED SystemEndpointSetWatermarksResponse {
    enum { cls = 0, cmd = 14 };
    std::uint16_t result;
};

struct PACKED SystemBootEvent {
    enum { cls = 0, cmd = 0 };
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
    enum { cls = 0, cmd = 1 };
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED SystemDebugEvent<0> {
    enum { cls = 0, cmd = 1 };
    std::uint8_t length;
};

struct PACKED SystemEndpointWatermarkRxEvent {
    enum { cls = 0, cmd = 2 };
    std::uint8_t endpoint;
    std::uint8_t data;
};

struct PACKED SystemEndpointWatermarkTxEvent {
    enum { cls = 0, cmd = 3 };
    std::uint8_t endpoint;
    std::uint8_t data;
};

struct PACKED SystemScriptFailureEvent {
    enum { cls = 0, cmd = 4 };
    std::uint16_t address;
    std::uint16_t reason;
};

struct PACKED SystemNoLicenseKeyEvent {
    enum { cls = 0, cmd = 5 };
};

namespace SystemEndpointsEnum {
enum {
    EndpointApi = 0,
    EndpointTest = 1,
    EndpointScript = 2,
    EndpointUsb = 3,
    EndpointUart0 = 4,
    EndpointUart1 = 5
};
}

struct PACKED FlashPsDefrag {
    enum { cls = 1, cmd = 0 };
};

struct PACKED FlashPsDefragResponse {
    enum { cls = 1, cmd = 0 };
};

struct PACKED FlashPsDump {
    enum { cls = 1, cmd = 1 };
};

struct PACKED FlashPsDumpResponse {
    enum { cls = 1, cmd = 1 };
};

struct PACKED FlashPsEraseAll {
    enum { cls = 1, cmd = 2 };
};

struct PACKED FlashPsEraseAllResponse {
    enum { cls = 1, cmd = 2 };
};

template <int N>
struct PACKED FlashPsSave {
    enum { cls = 1, cmd = 3 };
    std::uint16_t key;
    std::uint8_t length;
    std::uint8_t value[N];
};

template <>
struct PACKED FlashPsSave<0> {
    enum { cls = 1, cmd = 3 };
    std::uint16_t key;
    std::uint8_t length;
};

struct PACKED FlashPsSaveResponse {
    enum { cls = 1, cmd = 3 };
    std::uint16_t result;
};

struct PACKED FlashPsLoad {
    enum { cls = 1, cmd = 4 };
    std::uint16_t key;
};

template <int N>
struct PACKED FlashPsLoadResponse {
    enum { cls = 1, cmd = 4 };
    std::uint16_t result;
    std::uint8_t length;
    std::uint8_t value[N];
};

template <>
struct PACKED FlashPsLoadResponse<0> {
    enum { cls = 1, cmd = 4 };
    std::uint16_t result;
    std::uint8_t length;
};

struct PACKED FlashPsErase {
    enum { cls = 1, cmd = 5 };
    std::uint16_t key;
};

struct PACKED FlashPsEraseResponse {
    enum { cls = 1, cmd = 5 };
};

struct PACKED FlashErasePage {
    enum { cls = 1, cmd = 6 };
    std::uint8_t page;
};

struct PACKED FlashErasePageResponse {
    enum { cls = 1, cmd = 6 };
    std::uint16_t result;
};

template <int N>
struct PACKED FlashWriteWords {
    enum { cls = 1, cmd = 7 };
    std::uint16_t address;
    std::uint8_t length;
    std::uint8_t words[N];
};

template <>
struct PACKED FlashWriteWords<0> {
    enum { cls = 1, cmd = 7 };
    std::uint16_t address;
    std::uint8_t length;
};

struct PACKED FlashWriteWordsResponse {
    enum { cls = 1, cmd = 7 };
};

template <int N>
struct PACKED FlashPsKeyEvent {
    enum { cls = 1, cmd = 0 };
    std::uint16_t key;
    std::uint8_t length;
    std::uint8_t value[N];
};

template <>
struct PACKED FlashPsKeyEvent<0> {
    enum { cls = 1, cmd = 0 };
    std::uint16_t key;
    std::uint8_t length;
};

template <int N>
struct PACKED AttributesWrite {
    enum { cls = 2, cmd = 0 };
    std::uint16_t handle;
    std::uint8_t offset;
    std::uint8_t length;
    std::uint8_t value[N];
};

template <>
struct PACKED AttributesWrite<0> {
    enum { cls = 2, cmd = 0 };
    std::uint16_t handle;
    std::uint8_t offset;
    std::uint8_t length;
};

struct PACKED AttributesWriteResponse {
    enum { cls = 2, cmd = 0 };
    std::uint16_t result;
};

struct PACKED AttributesRead {
    enum { cls = 2, cmd = 1 };
    std::uint16_t handle;
    std::uint16_t offset;
};

template <int N>
struct PACKED AttributesReadResponse {
    enum { cls = 2, cmd = 1 };
    std::uint16_t handle;
    std::uint16_t offset;
    std::uint16_t result;
    std::uint8_t length;
    std::uint8_t value[N];
};

template <>
struct PACKED AttributesReadResponse<0> {
    enum { cls = 2, cmd = 1 };
    std::uint16_t handle;
    std::uint16_t offset;
    std::uint16_t result;
    std::uint8_t length;
};

struct PACKED AttributesReadType {
    enum { cls = 2, cmd = 2 };
    std::uint16_t handle;
};

template <int N>
struct PACKED AttributesReadTypeResponse {
    enum { cls = 2, cmd = 2 };
    std::uint16_t handle;
    std::uint16_t result;
    std::uint8_t length;
    std::uint8_t value[N];
};

template <>
struct PACKED AttributesReadTypeResponse<0> {
    enum { cls = 2, cmd = 2 };
    std::uint16_t handle;
    std::uint16_t result;
    std::uint8_t length;
};

template <int N>
struct PACKED AttributesUserReadResponse {
    enum { cls = 2, cmd = 3 };
    std::uint8_t connection;
    std::uint8_t att_error;
    std::uint8_t length;
    std::uint8_t value[N];
};

template <>
struct PACKED AttributesUserReadResponse<0> {
    enum { cls = 2, cmd = 3 };
    std::uint8_t connection;
    std::uint8_t att_error;
    std::uint8_t length;
};

struct PACKED AttributesUserReadResponseResponse {
    enum { cls = 2, cmd = 3 };
};

struct PACKED AttributesUserWriteResponse {
    enum { cls = 2, cmd = 4 };
    std::uint8_t connection;
    std::uint8_t att_error;
};

struct PACKED AttributesUserWriteResponseResponse {
    enum { cls = 2, cmd = 4 };
};

template <int N>
struct PACKED AttributesValueEvent {
    enum { cls = 2, cmd = 0 };
    std::uint8_t connection;
    std::uint8_t reason;
    std::uint16_t handle;
    std::uint16_t offset;
    std::uint8_t length;
    std::uint8_t value[N];
};

template <>
struct PACKED AttributesValueEvent<0> {
    enum { cls = 2, cmd = 0 };
    std::uint8_t connection;
    std::uint8_t reason;
    std::uint16_t handle;
    std::uint16_t offset;
    std::uint8_t length;
};

struct PACKED AttributesUserReadRequestEvent {
    enum { cls = 2, cmd = 1 };
    std::uint8_t connection;
    std::uint16_t handle;
    std::uint16_t offset;
    std::uint8_t maxsize;
};

struct PACKED AttributesStatusEvent {
    enum { cls = 2, cmd = 2 };
    std::uint16_t handle;
    std::uint8_t flags;
};

namespace AttributesAttributeChangeReasonEnum {
enum {
    AttributeChangeReasonWriteRequest = 0,
    AttributeChangeReasonWriteCommand = 1,
    AttributeChangeReasonWriteRequestUser = 2
};
}

namespace AttributesAttributeStatusFlagEnum {
enum {
    AttributeStatusFlagNotify = 1,
    AttributeStatusFlagIndicate = 2
};
}

struct PACKED ConnectionDisconnect {
    enum { cls = 3, cmd = 0 };
    std::uint8_t connection;
};

struct PACKED ConnectionDisconnectResponse {
    enum { cls = 3, cmd = 0 };
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED ConnectionGetRssi {
    enum { cls = 3, cmd = 1 };
    std::uint8_t connection;
};

struct PACKED ConnectionGetRssiResponse {
    enum { cls = 3, cmd = 1 };
    std::uint8_t connection;
    std::int8_t rssi;
};

struct PACKED ConnectionUpdate {
    enum { cls = 3, cmd = 2 };
    std::uint8_t connection;
    std::uint16_t interval_min;
    std::uint16_t interval_max;
    std::uint16_t latency;
    std::uint16_t timeout;
};

struct PACKED ConnectionUpdateResponse {
    enum { cls = 3, cmd = 2 };
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED ConnectionVersionUpdate {
    enum { cls = 3, cmd = 3 };
    std::uint8_t connection;
};

struct PACKED ConnectionVersionUpdateResponse {
    enum { cls = 3, cmd = 3 };
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED ConnectionChannelMapGet {
    enum { cls = 3, cmd = 4 };
    std::uint8_t connection;
};

template <int N>
struct PACKED ConnectionChannelMapGetResponse {
    enum { cls = 3, cmd = 4 };
    std::uint8_t connection;
    std::uint8_t length;
    std::uint8_t map[N];
};

template <>
struct PACKED ConnectionChannelMapGetResponse<0> {
    enum { cls = 3, cmd = 4 };
    std::uint8_t connection;
    std::uint8_t length;
};

template <int N>
struct PACKED ConnectionChannelMapSet {
    enum { cls = 3, cmd = 5 };
    std::uint8_t connection;
    std::uint8_t length;
    std::uint8_t map[N];
};

template <>
struct PACKED ConnectionChannelMapSet<0> {
    enum { cls = 3, cmd = 5 };
    std::uint8_t connection;
    std::uint8_t length;
};

struct PACKED ConnectionChannelMapSetResponse {
    enum { cls = 3, cmd = 5 };
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED ConnectionFeaturesGet {
    enum { cls = 3, cmd = 6 };
    std::uint8_t connection;
};

struct PACKED ConnectionFeaturesGetResponse {
    enum { cls = 3, cmd = 6 };
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED ConnectionGetStatus {
    enum { cls = 3, cmd = 7 };
    std::uint8_t connection;
};

struct PACKED ConnectionGetStatusResponse {
    enum { cls = 3, cmd = 7 };
    std::uint8_t connection;
};

template <int N>
struct PACKED ConnectionRawTx {
    enum { cls = 3, cmd = 8 };
    std::uint8_t connection;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED ConnectionRawTx<0> {
    enum { cls = 3, cmd = 8 };
    std::uint8_t connection;
    std::uint8_t length;
};

struct PACKED ConnectionRawTxResponse {
    enum { cls = 3, cmd = 8 };
    std::uint8_t connection;
};

struct PACKED ConnectionStatusEvent {
    enum { cls = 3, cmd = 0 };
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
    enum { cls = 3, cmd = 1 };
    std::uint8_t connection;
    std::uint8_t vers_nr;
    std::uint16_t comp_id;
    std::uint16_t sub_vers_nr;
};

template <int N>
struct PACKED ConnectionFeatureIndEvent {
    enum { cls = 3, cmd = 2 };
    std::uint8_t connection;
    std::uint8_t length;
    std::uint8_t features[N];
};

template <>
struct PACKED ConnectionFeatureIndEvent<0> {
    enum { cls = 3, cmd = 2 };
    std::uint8_t connection;
    std::uint8_t length;
};

template <int N>
struct PACKED ConnectionRawRxEvent {
    enum { cls = 3, cmd = 3 };
    std::uint8_t connection;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED ConnectionRawRxEvent<0> {
    enum { cls = 3, cmd = 3 };
    std::uint8_t connection;
    std::uint8_t length;
};

struct PACKED ConnectionDisconnectedEvent {
    enum { cls = 3, cmd = 4 };
    std::uint8_t connection;
    std::uint16_t reason;
};

namespace ConnectionConnstatusEnum {
enum {
    Connected = 1,
    Encrypted = 2,
    Completed = 4,
    ParametersChange = 8
};
}

template <int N>
struct PACKED AttclientFindByTypeValue {
    enum { cls = 4, cmd = 0 };
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint16_t uuid;
    std::uint8_t length;
    std::uint8_t value[N];
};

template <>
struct PACKED AttclientFindByTypeValue<0> {
    enum { cls = 4, cmd = 0 };
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint16_t uuid;
    std::uint8_t length;
};

struct PACKED AttclientFindByTypeValueResponse {
    enum { cls = 4, cmd = 0 };
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientReadByGroupType {
    enum { cls = 4, cmd = 1 };
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint8_t length;
    std::uint8_t uuid[N];
};

template <>
struct PACKED AttclientReadByGroupType<0> {
    enum { cls = 4, cmd = 1 };
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint8_t length;
};

struct PACKED AttclientReadByGroupTypeResponse {
    enum { cls = 4, cmd = 1 };
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientReadByType {
    enum { cls = 4, cmd = 2 };
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint8_t length;
    std::uint8_t uuid[N];
};

template <>
struct PACKED AttclientReadByType<0> {
    enum { cls = 4, cmd = 2 };
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint8_t length;
};

struct PACKED AttclientReadByTypeResponse {
    enum { cls = 4, cmd = 2 };
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED AttclientFindInformation {
    enum { cls = 4, cmd = 3 };
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
};

struct PACKED AttclientFindInformationResponse {
    enum { cls = 4, cmd = 3 };
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED AttclientReadByHandle {
    enum { cls = 4, cmd = 4 };
    std::uint8_t connection;
    std::uint16_t chrhandle;
};

struct PACKED AttclientReadByHandleResponse {
    enum { cls = 4, cmd = 4 };
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientAttributeWrite {
    enum { cls = 4, cmd = 5 };
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED AttclientAttributeWrite<0> {
    enum { cls = 4, cmd = 5 };
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint8_t length;
};

struct PACKED AttclientAttributeWriteResponse {
    enum { cls = 4, cmd = 5 };
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientWriteCommand {
    enum { cls = 4, cmd = 6 };
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED AttclientWriteCommand<0> {
    enum { cls = 4, cmd = 6 };
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint8_t length;
};

struct PACKED AttclientWriteCommandResponse {
    enum { cls = 4, cmd = 6 };
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED AttclientIndicateConfirm {
    enum { cls = 4, cmd = 7 };
    std::uint8_t connection;
};

struct PACKED AttclientIndicateConfirmResponse {
    enum { cls = 4, cmd = 7 };
    std::uint16_t result;
};

struct PACKED AttclientReadLong {
    enum { cls = 4, cmd = 8 };
    std::uint8_t connection;
    std::uint16_t chrhandle;
};

struct PACKED AttclientReadLongResponse {
    enum { cls = 4, cmd = 8 };
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientPrepareWrite {
    enum { cls = 4, cmd = 9 };
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint16_t offset;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED AttclientPrepareWrite<0> {
    enum { cls = 4, cmd = 9 };
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint16_t offset;
    std::uint8_t length;
};

struct PACKED AttclientPrepareWriteResponse {
    enum { cls = 4, cmd = 9 };
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED AttclientExecuteWrite {
    enum { cls = 4, cmd = 10 };
    std::uint8_t connection;
    std::uint8_t commit;
};

struct PACKED AttclientExecuteWriteResponse {
    enum { cls = 4, cmd = 10 };
    std::uint8_t connection;
    std::uint16_t result;
};

template <int N>
struct PACKED AttclientReadMultiple {
    enum { cls = 4, cmd = 11 };
    std::uint8_t connection;
    std::uint8_t length;
    std::uint8_t handles[N];
};

template <>
struct PACKED AttclientReadMultiple<0> {
    enum { cls = 4, cmd = 11 };
    std::uint8_t connection;
    std::uint8_t length;
};

struct PACKED AttclientReadMultipleResponse {
    enum { cls = 4, cmd = 11 };
    std::uint8_t connection;
    std::uint16_t result;
};

struct PACKED AttclientIndicatedEvent {
    enum { cls = 4, cmd = 0 };
    std::uint8_t connection;
    std::uint16_t attrhandle;
};

struct PACKED AttclientProcedureCompletedEvent {
    enum { cls = 4, cmd = 1 };
    std::uint8_t connection;
    std::uint16_t result;
    std::uint16_t chrhandle;
};

template <int N>
struct PACKED AttclientGroupFoundEvent {
    enum { cls = 4, cmd = 2 };
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint8_t length;
    std::uint8_t uuid[N];
};

template <>
struct PACKED AttclientGroupFoundEvent<0> {
    enum { cls = 4, cmd = 2 };
    std::uint8_t connection;
    std::uint16_t start;
    std::uint16_t end;
    std::uint8_t length;
};

template <int N>
struct PACKED AttclientAttributeFoundEvent {
    enum { cls = 4, cmd = 3 };
    std::uint8_t connection;
    std::uint16_t chrdecl;
    std::uint16_t value;
    std::uint8_t properties;
    std::uint8_t length;
    std::uint8_t uuid[N];
};

template <>
struct PACKED AttclientAttributeFoundEvent<0> {
    enum { cls = 4, cmd = 3 };
    std::uint8_t connection;
    std::uint16_t chrdecl;
    std::uint16_t value;
    std::uint8_t properties;
    std::uint8_t length;
};

template <int N>
struct PACKED AttclientFindInformationFoundEvent {
    enum { cls = 4, cmd = 4 };
    std::uint8_t connection;
    std::uint16_t chrhandle;
    std::uint8_t length;
    std::uint8_t uuid[N];
};

template <>
struct PACKED AttclientFindInformationFoundEvent<0> {
    enum { cls = 4, cmd = 4 };
    std::uint8_t connection;
    std::uint16_t chrhandle;
    std::uint8_t length;
};

template <int N>
struct PACKED AttclientAttributeValueEvent {
    enum { cls = 4, cmd = 5 };
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint8_t type;
    std::uint8_t length;
    std::uint8_t value[N];
};

template <>
struct PACKED AttclientAttributeValueEvent<0> {
    enum { cls = 4, cmd = 5 };
    std::uint8_t connection;
    std::uint16_t atthandle;
    std::uint8_t type;
    std::uint8_t length;
};

template <int N>
struct PACKED AttclientReadMultipleResponseEvent {
    enum { cls = 4, cmd = 6 };
    std::uint8_t connection;
    std::uint8_t length;
    std::uint8_t handles[N];
};

template <>
struct PACKED AttclientReadMultipleResponseEvent<0> {
    enum { cls = 4, cmd = 6 };
    std::uint8_t connection;
    std::uint8_t length;
};

namespace AttclientAttributeValueTypesEnum {
enum {
    AttributeValueTypeRead = 0,
    AttributeValueTypeNotify = 1,
    AttributeValueTypeIndicate = 2,
    AttributeValueTypeReadByType = 3,
    AttributeValueTypeReadBlob = 4,
    AttributeValueTypeIndicateRspReq = 5
};
}

struct PACKED SmEncryptStart {
    enum { cls = 5, cmd = 0 };
    std::uint8_t handle;
    std::uint8_t bonding;
};

struct PACKED SmEncryptStartResponse {
    enum { cls = 5, cmd = 0 };
    std::uint8_t handle;
    std::uint16_t result;
};

struct PACKED SmSetBondableMode {
    enum { cls = 5, cmd = 1 };
    std::uint8_t bondable;
};

struct PACKED SmSetBondableModeResponse {
    enum { cls = 5, cmd = 1 };
};

struct PACKED SmDeleteBonding {
    enum { cls = 5, cmd = 2 };
    std::uint8_t handle;
};

struct PACKED SmDeleteBondingResponse {
    enum { cls = 5, cmd = 2 };
    std::uint16_t result;
};

struct PACKED SmSetParameters {
    enum { cls = 5, cmd = 3 };
    std::uint8_t mitm;
    std::uint8_t min_key_size;
    std::uint8_t io_capabilities;
};

struct PACKED SmSetParametersResponse {
    enum { cls = 5, cmd = 3 };
};

struct PACKED SmPasskeyEntry {
    enum { cls = 5, cmd = 4 };
    std::uint8_t handle;
    std::uint32_t passkey;
};

struct PACKED SmPasskeyEntryResponse {
    enum { cls = 5, cmd = 4 };
    std::uint16_t result;
};

struct PACKED SmGetBonds {
    enum { cls = 5, cmd = 5 };
};

struct PACKED SmGetBondsResponse {
    enum { cls = 5, cmd = 5 };
    std::uint8_t bonds;
};

template <int N>
struct PACKED SmSetOobData {
    enum { cls = 5, cmd = 6 };
    std::uint8_t length;
    std::uint8_t oob[N];
};

template <>
struct PACKED SmSetOobData<0> {
    enum { cls = 5, cmd = 6 };
    std::uint8_t length;
};

struct PACKED SmSetOobDataResponse {
    enum { cls = 5, cmd = 6 };
};

template <int N>
struct PACKED SmSmpDataEvent {
    enum { cls = 5, cmd = 0 };
    std::uint8_t handle;
    std::uint8_t packet;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED SmSmpDataEvent<0> {
    enum { cls = 5, cmd = 0 };
    std::uint8_t handle;
    std::uint8_t packet;
    std::uint8_t length;
};

struct PACKED SmBondingFailEvent {
    enum { cls = 5, cmd = 1 };
    std::uint8_t handle;
    std::uint16_t result;
};

struct PACKED SmPasskeyDisplayEvent {
    enum { cls = 5, cmd = 2 };
    std::uint8_t handle;
    std::uint32_t passkey;
};

struct PACKED SmPasskeyRequestEvent {
    enum { cls = 5, cmd = 3 };
    std::uint8_t handle;
};

struct PACKED SmBondStatusEvent {
    enum { cls = 5, cmd = 4 };
    std::uint8_t bond;
    std::uint8_t keysize;
    std::uint8_t mitm;
    std::uint8_t keys;
};

namespace SmBondingKeyEnum {
enum {
    BondingKeyLtk = 0x01,
    BondingKeyAddrPublic = 0x02,
    BondingKeyAddrStatic = 0x04,
    BondingKeyIrk = 0x08,
    BondingKeyEdivrand = 0x10,
    BondingKeyCsrk = 0x20,
    BondingKeyMasterid = 0x40
};
}

namespace SmIoCapabilityEnum {
enum {
    IoCapabilityDisplayonly = 0,
    IoCapabilityDisplayyesno = 1,
    IoCapabilityKeyboardonly = 2,
    IoCapabilityNoinputnooutput = 3,
    IoCapabilityKeyboarddisplay = 4
};
}

struct PACKED GapSetPrivacyFlags {
    enum { cls = 6, cmd = 0 };
    std::uint8_t peripheral_privacy;
    std::uint8_t central_privacy;
};

struct PACKED GapSetPrivacyFlagsResponse {
    enum { cls = 6, cmd = 0 };
};

struct PACKED GapSetMode {
    enum { cls = 6, cmd = 1 };
    std::uint8_t discover;
    std::uint8_t connect;
};

struct PACKED GapSetModeResponse {
    enum { cls = 6, cmd = 1 };
    std::uint16_t result;
};

struct PACKED GapDiscover {
    enum { cls = 6, cmd = 2 };
    std::uint8_t mode;
};

struct PACKED GapDiscoverResponse {
    enum { cls = 6, cmd = 2 };
    std::uint16_t result;
};

struct PACKED GapConnectDirect {
    enum { cls = 6, cmd = 3 };
    std::uint8_t address[6];
    std::uint8_t addr_type;
    std::uint16_t conn_interval_min;
    std::uint16_t conn_interval_max;
    std::uint16_t timeout;
    std::uint16_t latency;
};

struct PACKED GapConnectDirectResponse {
    enum { cls = 6, cmd = 3 };
    std::uint16_t result;
    std::uint8_t connection_handle;
};

struct PACKED GapEndProcedure {
    enum { cls = 6, cmd = 4 };
};

struct PACKED GapEndProcedureResponse {
    enum { cls = 6, cmd = 4 };
    std::uint16_t result;
};

struct PACKED GapConnectSelective {
    enum { cls = 6, cmd = 5 };
    std::uint16_t conn_interval_min;
    std::uint16_t conn_interval_max;
    std::uint16_t timeout;
    std::uint16_t latency;
};

struct PACKED GapConnectSelectiveResponse {
    enum { cls = 6, cmd = 5 };
    std::uint16_t result;
    std::uint8_t connection_handle;
};

struct PACKED GapSetFiltering {
    enum { cls = 6, cmd = 6 };
    std::uint8_t scan_policy;
    std::uint8_t adv_policy;
    std::uint8_t scan_duplicate_filtering;
};

struct PACKED GapSetFilteringResponse {
    enum { cls = 6, cmd = 6 };
    std::uint16_t result;
};

struct PACKED GapSetScanParameters {
    enum { cls = 6, cmd = 7 };
    std::uint16_t scan_interval;
    std::uint16_t scan_window;
    std::uint8_t active;
};

struct PACKED GapSetScanParametersResponse {
    enum { cls = 6, cmd = 7 };
    std::uint16_t result;
};

struct PACKED GapSetAdvParameters {
    enum { cls = 6, cmd = 8 };
    std::uint16_t adv_interval_min;
    std::uint16_t adv_interval_max;
    std::uint8_t adv_channels;
};

struct PACKED GapSetAdvParametersResponse {
    enum { cls = 6, cmd = 8 };
    std::uint16_t result;
};

template <int N>
struct PACKED GapSetAdvData {
    enum { cls = 6, cmd = 9 };
    std::uint8_t set_scanrsp;
    std::uint8_t length;
    std::uint8_t adv_data[N];
};

template <>
struct PACKED GapSetAdvData<0> {
    enum { cls = 6, cmd = 9 };
    std::uint8_t set_scanrsp;
    std::uint8_t length;
};

struct PACKED GapSetAdvDataResponse {
    enum { cls = 6, cmd = 9 };
    std::uint16_t result;
};

struct PACKED GapSetDirectedConnectableMode {
    enum { cls = 6, cmd = 10 };
    std::uint8_t address[6];
    std::uint8_t addr_type;
};

struct PACKED GapSetDirectedConnectableModeResponse {
    enum { cls = 6, cmd = 10 };
    std::uint16_t result;
};

template <int N>
struct PACKED GapScanResponseEvent {
    enum { cls = 6, cmd = 0 };
    std::int8_t rssi;
    std::uint8_t packet_type;
    std::uint8_t sender[6];
    std::uint8_t address_type;
    std::uint8_t bond;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED GapScanResponseEvent<0> {
    enum { cls = 6, cmd = 0 };
    std::int8_t rssi;
    std::uint8_t packet_type;
    std::uint8_t sender[6];
    std::uint8_t address_type;
    std::uint8_t bond;
    std::uint8_t length;
};

struct PACKED GapModeChangedEvent {
    enum { cls = 6, cmd = 1 };
    std::uint8_t discover;
    std::uint8_t connect;
};

namespace GapAddressTypeEnum {
enum {
    AddressTypePublic = 0,
    AddressTypeRandom = 1
};
}

namespace GapDiscoverableModeEnum {
enum {
    NonDiscoverable = 0,
    LimitedDiscoverable = 1,
    GeneralDiscoverable = 2,
    Broadcast = 3,
    UserData = 4
};
}

namespace GapConnectableModeEnum {
enum {
    NonConnectable = 0,
    DirectedConnectable = 1,
    UndirectedConnectable = 2,
    ScannableConnectable = 3
};
}

namespace GapDiscoverModeEnum {
enum {
    DiscoverLimited = 0,
    DiscoverGeneric = 1,
    DiscoverObservation = 2
};
}

namespace GapAdTypesEnum {
enum {
    AdTypeNone = 0,
    AdTypeFlags = 1,
    AdTypeServices16BitMore = 2,
    AdTypeServices16BitAll = 3,
    AdTypeServices32BitMore = 4,
    AdTypeServices32BitAll = 5,
    AdTypeServices128BitMore = 6,
    AdTypeServices128BitAll = 7,
    AdTypeLocalnameShort = 8,
    AdTypeLocalnameComplete = 9,
    AdTypeTxpower = 10
};
}

namespace GapAdvertisingPolicyEnum {
enum {
    AdvPolicyAll = 0,
    AdvPolicyWhitelistScan = 1,
    AdvPolicyWhitelistConnect = 2,
    AdvPolicyWhitelistAll = 3
};
}

namespace GapScanPolicyEnum {
enum {
    ScanPolicyAll = 0,
    ScanPolicyWhitelist = 1
};
}

struct PACKED HardwareIoPortConfigIrq {
    enum { cls = 7, cmd = 0 };
    std::uint8_t port;
    std::uint8_t enable_bits;
    std::uint8_t falling_edge;
};

struct PACKED HardwareIoPortConfigIrqResponse {
    enum { cls = 7, cmd = 0 };
    std::uint16_t result;
};

struct PACKED HardwareSetSoftTimer {
    enum { cls = 7, cmd = 1 };
    std::uint32_t time;
    std::uint8_t handle;
    std::uint8_t single_shot;
};

struct PACKED HardwareSetSoftTimerResponse {
    enum { cls = 7, cmd = 1 };
    std::uint16_t result;
};

struct PACKED HardwareAdcRead {
    enum { cls = 7, cmd = 2 };
    std::uint8_t input;
    std::uint8_t decimation;
    std::uint8_t reference_selection;
};

struct PACKED HardwareAdcReadResponse {
    enum { cls = 7, cmd = 2 };
    std::uint16_t result;
};

struct PACKED HardwareIoPortConfigDirection {
    enum { cls = 7, cmd = 3 };
    std::uint8_t port;
    std::uint8_t direction;
};

struct PACKED HardwareIoPortConfigDirectionResponse {
    enum { cls = 7, cmd = 3 };
    std::uint16_t result;
};

struct PACKED HardwareIoPortConfigFunction {
    enum { cls = 7, cmd = 4 };
    std::uint8_t port;
    std::uint8_t function;
};

struct PACKED HardwareIoPortConfigFunctionResponse {
    enum { cls = 7, cmd = 4 };
    std::uint16_t result;
};

struct PACKED HardwareIoPortConfigPull {
    enum { cls = 7, cmd = 5 };
    std::uint8_t port;
    std::uint8_t tristate_mask;
    std::uint8_t pull_up;
};

struct PACKED HardwareIoPortConfigPullResponse {
    enum { cls = 7, cmd = 5 };
    std::uint16_t result;
};

struct PACKED HardwareIoPortWrite {
    enum { cls = 7, cmd = 6 };
    std::uint8_t port;
    std::uint8_t mask;
    std::uint8_t data;
};

struct PACKED HardwareIoPortWriteResponse {
    enum { cls = 7, cmd = 6 };
    std::uint16_t result;
};

struct PACKED HardwareIoPortRead {
    enum { cls = 7, cmd = 7 };
    std::uint8_t port;
    std::uint8_t mask;
};

struct PACKED HardwareIoPortReadResponse {
    enum { cls = 7, cmd = 7 };
    std::uint16_t result;
    std::uint8_t port;
    std::uint8_t data;
};

struct PACKED HardwareSpiConfig {
    enum { cls = 7, cmd = 8 };
    std::uint8_t channel;
    std::uint8_t polarity;
    std::uint8_t phase;
    std::uint8_t bit_order;
    std::uint8_t baud_e;
    std::uint8_t baud_m;
};

struct PACKED HardwareSpiConfigResponse {
    enum { cls = 7, cmd = 8 };
    std::uint16_t result;
};

template <int N>
struct PACKED HardwareSpiTransfer {
    enum { cls = 7, cmd = 9 };
    std::uint8_t channel;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED HardwareSpiTransfer<0> {
    enum { cls = 7, cmd = 9 };
    std::uint8_t channel;
    std::uint8_t length;
};

template <int N>
struct PACKED HardwareSpiTransferResponse {
    enum { cls = 7, cmd = 9 };
    std::uint16_t result;
    std::uint8_t channel;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED HardwareSpiTransferResponse<0> {
    enum { cls = 7, cmd = 9 };
    std::uint16_t result;
    std::uint8_t channel;
    std::uint8_t length;
};

struct PACKED HardwareI2CRead {
    enum { cls = 7, cmd = 10 };
    std::uint8_t address;
    std::uint8_t stop;
    std::uint8_t length;
};

template <int N>
struct PACKED HardwareI2CReadResponse {
    enum { cls = 7, cmd = 10 };
    std::uint16_t result;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED HardwareI2CReadResponse<0> {
    enum { cls = 7, cmd = 10 };
    std::uint16_t result;
    std::uint8_t length;
};

template <int N>
struct PACKED HardwareI2CWrite {
    enum { cls = 7, cmd = 11 };
    std::uint8_t address;
    std::uint8_t stop;
    std::uint8_t length;
    std::uint8_t data[N];
};

template <>
struct PACKED HardwareI2CWrite<0> {
    enum { cls = 7, cmd = 11 };
    std::uint8_t address;
    std::uint8_t stop;
    std::uint8_t length;
};

struct PACKED HardwareI2CWriteResponse {
    enum { cls = 7, cmd = 11 };
    std::uint8_t written;
};

struct PACKED HardwareSetTxpower {
    enum { cls = 7, cmd = 12 };
    std::uint8_t power;
};

struct PACKED HardwareSetTxpowerResponse {
    enum { cls = 7, cmd = 12 };
};

struct PACKED HardwareTimerComparator {
    enum { cls = 7, cmd = 13 };
    std::uint8_t timer;
    std::uint8_t channel;
    std::uint8_t mode;
    std::uint16_t comparator_value;
};

struct PACKED HardwareTimerComparatorResponse {
    enum { cls = 7, cmd = 13 };
    std::uint16_t result;
};

struct PACKED HardwareIoPortStatusEvent {
    enum { cls = 7, cmd = 0 };
    std::uint32_t timestamp;
    std::uint8_t port;
    std::uint8_t irq;
    std::uint8_t state;
};

struct PACKED HardwareSoftTimerEvent {
    enum { cls = 7, cmd = 1 };
    std::uint8_t handle;
};

struct PACKED HardwareAdcResultEvent {
    enum { cls = 7, cmd = 2 };
    std::uint8_t input;
    std::int16_t value;
};

struct PACKED TestPhyTx {
    enum { cls = 8, cmd = 0 };
    std::uint8_t channel;
    std::uint8_t length;
    std::uint8_t type;
};

struct PACKED TestPhyTxResponse {
    enum { cls = 8, cmd = 0 };
};

struct PACKED TestPhyRx {
    enum { cls = 8, cmd = 1 };
    std::uint8_t channel;
};

struct PACKED TestPhyRxResponse {
    enum { cls = 8, cmd = 1 };
};

struct PACKED TestPhyEnd {
    enum { cls = 8, cmd = 2 };
};

struct PACKED TestPhyEndResponse {
    enum { cls = 8, cmd = 2 };
    std::uint16_t counter;
};

struct PACKED TestPhyReset {
    enum { cls = 8, cmd = 3 };
};

struct PACKED TestPhyResetResponse {
    enum { cls = 8, cmd = 3 };
};

struct PACKED TestGetChannelMap {
    enum { cls = 8, cmd = 4 };
};

template <int N>
struct PACKED TestGetChannelMapResponse {
    enum { cls = 8, cmd = 4 };
    std::uint8_t length;
    std::uint8_t channel_map[N];
};

template <>
struct PACKED TestGetChannelMapResponse<0> {
    enum { cls = 8, cmd = 4 };
    std::uint8_t length;
};

template <int N>
struct PACKED TestDebug {
    enum { cls = 8, cmd = 5 };
    std::uint8_t length;
    std::uint8_t input[N];
};

template <>
struct PACKED TestDebug<0> {
    enum { cls = 8, cmd = 5 };
    std::uint8_t length;
};

template <int N>
struct PACKED TestDebugResponse {
    enum { cls = 8, cmd = 5 };
    std::uint8_t length;
    std::uint8_t output[N];
};

template <>
struct PACKED TestDebugResponse<0> {
    enum { cls = 8, cmd = 5 };
    std::uint8_t length;
};

#endif // BLEAPI_H