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
    uint8_t boot_in_dfu;
};

struct PACKED SystemResetResponse {
};

Header getHeader(SystemReset *)
{
    return Header{0, 0, 0, sizeof(SystemReset), 0, 0};
}

struct PACKED SystemHello {
};

struct PACKED SystemHelloResponse {
};

Header getHeader(SystemHello *)
{
    return Header{0, 0, 0, sizeof(SystemHello), 0, 1};
}

struct PACKED SystemAddressGet {
};

struct PACKED SystemAddressGetResponse {
    uint8_t address[6];
};

Header getHeader(SystemAddressGet *)
{
    return Header{0, 0, 0, sizeof(SystemAddressGet), 0, 2};
}

struct PACKED SystemRegWrite {
    uint16_t address;
    uint8_t value;
};

struct PACKED SystemRegWriteResponse {
    uint16_t result;
};

Header getHeader(SystemRegWrite *)
{
    return Header{0, 0, 0, sizeof(SystemRegWrite), 0, 3};
}

struct PACKED SystemRegRead {
    uint16_t address;
};

struct PACKED SystemRegReadResponse {
    uint16_t address;
    uint8_t value;
};

Header getHeader(SystemRegRead *)
{
    return Header{0, 0, 0, sizeof(SystemRegRead), 0, 4};
}

struct PACKED SystemGetCounters {
};

struct PACKED SystemGetCountersResponse {
    uint8_t txok;
    uint8_t txretry;
    uint8_t rxok;
    uint8_t rxfail;
    uint8_t mbuf;
};

Header getHeader(SystemGetCounters *)
{
    return Header{0, 0, 0, sizeof(SystemGetCounters), 0, 5};
}

struct PACKED SystemGetConnections {
};

struct PACKED SystemGetConnectionsResponse {
    uint8_t maxconn;
};

Header getHeader(SystemGetConnections *)
{
    return Header{0, 0, 0, sizeof(SystemGetConnections), 0, 6};
}

template <int N>
struct PACKED SystemReadMemory {
    uint32_t address;
    uint8_t length;
};

template <int N>
struct PACKED SystemReadMemoryResponse {
    uint32_t address;
    uint8_t data[N];
};

template <int N>
Header getHeader(SystemReadMemory<N> *)
{
    return Header{0, 0, 0, sizeof(SystemReadMemory<N>), 0, 7};
}

struct PACKED SystemGetInfo {
};

struct PACKED SystemGetInfoResponse {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
    uint16_t build;
    uint16_t ll_version;
    uint8_t protocol_version;
    uint8_t hw;
};

Header getHeader(SystemGetInfo *)
{
    return Header{0, 0, 0, sizeof(SystemGetInfo), 0, 8};
}

template <int N>
struct PACKED SystemEndpointTx {
    uint8_t endpoint;
    uint8_t data[N];
};

struct PACKED SystemEndpointTxResponse {
    uint16_t result;
};

template <int N>
Header getHeader(SystemEndpointTx<N> *)
{
    return Header{0, 0, 0, sizeof(SystemEndpointTx<N>), 0, 9};
}

struct PACKED SystemWhitelistAppend {
    uint8_t address[6];
    uint8_t address_type;
};

struct PACKED SystemWhitelistAppendResponse {
    uint16_t result;
};

Header getHeader(SystemWhitelistAppend *)
{
    return Header{0, 0, 0, sizeof(SystemWhitelistAppend), 0, 10};
}

struct PACKED SystemWhitelistRemove {
    uint8_t address[6];
    uint8_t address_type;
};

struct PACKED SystemWhitelistRemoveResponse {
    uint16_t result;
};

Header getHeader(SystemWhitelistRemove *)
{
    return Header{0, 0, 0, sizeof(SystemWhitelistRemove), 0, 11};
}

struct PACKED SystemWhitelistClear {
};

struct PACKED SystemWhitelistClearResponse {
};

Header getHeader(SystemWhitelistClear *)
{
    return Header{0, 0, 0, sizeof(SystemWhitelistClear), 0, 12};
}

template <int N>
struct PACKED SystemEndpointRx {
    uint8_t endpoint;
    uint8_t size;
};

template <int N>
struct PACKED SystemEndpointRxResponse {
    uint16_t result;
    uint8_t data[N];
};

template <int N>
Header getHeader(SystemEndpointRx<N> *)
{
    return Header{0, 0, 0, sizeof(SystemEndpointRx<N>), 0, 13};
}

struct PACKED SystemEndpointSetWatermarks {
    uint8_t endpoint;
    uint8_t rx;
    uint8_t tx;
};

struct PACKED SystemEndpointSetWatermarksResponse {
    uint16_t result;
};

Header getHeader(SystemEndpointSetWatermarks *)
{
    return Header{0, 0, 0, sizeof(SystemEndpointSetWatermarks), 0, 14};
}

struct PACKED SystemBootEvent {
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
    uint8_t data[N];
};

struct PACKED SystemEndpointWatermarkRxEvent {
    uint8_t endpoint;
    uint8_t data;
};

struct PACKED SystemEndpointWatermarkTxEvent {
    uint8_t endpoint;
    uint8_t data;
};

struct PACKED SystemScriptFailureEvent {
    uint16_t address;
    uint16_t reason;
};

struct PACKED SystemNoLicenseKeyEvent {
};

struct PACKED FlashPsDefrag {
};

struct PACKED FlashPsDefragResponse {
};

Header getHeader(FlashPsDefrag *)
{
    return Header{0, 0, 0, sizeof(FlashPsDefrag), 1, 0};
}

struct PACKED FlashPsDump {
};

struct PACKED FlashPsDumpResponse {
};

Header getHeader(FlashPsDump *)
{
    return Header{0, 0, 0, sizeof(FlashPsDump), 1, 1};
}

struct PACKED FlashPsEraseAll {
};

struct PACKED FlashPsEraseAllResponse {
};

Header getHeader(FlashPsEraseAll *)
{
    return Header{0, 0, 0, sizeof(FlashPsEraseAll), 1, 2};
}

template <int N>
struct PACKED FlashPsSave {
    uint16_t key;
    uint8_t value[N];
};

struct PACKED FlashPsSaveResponse {
    uint16_t result;
};

template <int N>
Header getHeader(FlashPsSave<N> *)
{
    return Header{0, 0, 0, sizeof(FlashPsSave<N>), 1, 3};
}

template <int N>
struct PACKED FlashPsLoad {
    uint16_t key;
};

template <int N>
struct PACKED FlashPsLoadResponse {
    uint16_t result;
    uint8_t value[N];
};

template <int N>
Header getHeader(FlashPsLoad<N> *)
{
    return Header{0, 0, 0, sizeof(FlashPsLoad<N>), 1, 4};
}

struct PACKED FlashPsErase {
    uint16_t key;
};

struct PACKED FlashPsEraseResponse {
};

Header getHeader(FlashPsErase *)
{
    return Header{0, 0, 0, sizeof(FlashPsErase), 1, 5};
}

struct PACKED FlashErasePage {
    uint8_t page;
};

struct PACKED FlashErasePageResponse {
    uint16_t result;
};

Header getHeader(FlashErasePage *)
{
    return Header{0, 0, 0, sizeof(FlashErasePage), 1, 6};
}

template <int N>
struct PACKED FlashWriteWords {
    uint16_t address;
    uint8_t words[N];
};

struct PACKED FlashWriteWordsResponse {
};

template <int N>
Header getHeader(FlashWriteWords<N> *)
{
    return Header{0, 0, 0, sizeof(FlashWriteWords<N>), 1, 7};
}

template <int N>
struct PACKED FlashPsKeyEvent {
    uint16_t key;
    uint8_t value[N];
};

template <int N>
struct PACKED AttributesWrite {
    uint16_t handle;
    uint8_t offset;
    uint8_t value[N];
};

struct PACKED AttributesWriteResponse {
    uint16_t result;
};

template <int N>
Header getHeader(AttributesWrite<N> *)
{
    return Header{0, 0, 0, sizeof(AttributesWrite<N>), 2, 0};
}

template <int N>
struct PACKED AttributesRead {
    uint16_t handle;
    uint16_t offset;
};

template <int N>
struct PACKED AttributesReadResponse {
    uint16_t handle;
    uint16_t offset;
    uint16_t result;
    uint8_t value[N];
};

template <int N>
Header getHeader(AttributesRead<N> *)
{
    return Header{0, 0, 0, sizeof(AttributesRead<N>), 2, 1};
}

template <int N>
struct PACKED AttributesReadType {
    uint16_t handle;
};

template <int N>
struct PACKED AttributesReadTypeResponse {
    uint16_t handle;
    uint16_t result;
    uint8_t value[N];
};

template <int N>
Header getHeader(AttributesReadType<N> *)
{
    return Header{0, 0, 0, sizeof(AttributesReadType<N>), 2, 2};
}

template <int N>
struct PACKED AttributesUserReadResponse {
    uint8_t connection;
    uint8_t att_error;
    uint8_t value[N];
};

struct PACKED AttributesUserReadResponseResponse {
};

template <int N>
Header getHeader(AttributesUserReadResponse<N> *)
{
    return Header{0, 0, 0, sizeof(AttributesUserReadResponse<N>), 2, 3};
}

struct PACKED AttributesUserWriteResponse {
    uint8_t connection;
    uint8_t att_error;
};

struct PACKED AttributesUserWriteResponseResponse {
};

Header getHeader(AttributesUserWriteResponse *)
{
    return Header{0, 0, 0, sizeof(AttributesUserWriteResponse), 2, 4};
}

template <int N>
struct PACKED AttributesValueEvent {
    uint8_t connection;
    uint8_t reason;
    uint16_t handle;
    uint16_t offset;
    uint8_t value[N];
};

struct PACKED AttributesUserReadRequestEvent {
    uint8_t connection;
    uint16_t handle;
    uint16_t offset;
    uint8_t maxsize;
};

struct PACKED AttributesStatusEvent {
    uint16_t handle;
    uint8_t flags;
};

struct PACKED ConnectionDisconnect {
    uint8_t connection;
};

struct PACKED ConnectionDisconnectResponse {
    uint8_t connection;
    uint16_t result;
};

Header getHeader(ConnectionDisconnect *)
{
    return Header{0, 0, 0, sizeof(ConnectionDisconnect), 3, 0};
}

struct PACKED ConnectionGetRssi {
    uint8_t connection;
};

struct PACKED ConnectionGetRssiResponse {
    uint8_t connection;
    int8_t rssi;
};

Header getHeader(ConnectionGetRssi *)
{
    return Header{0, 0, 0, sizeof(ConnectionGetRssi), 3, 1};
}

struct PACKED ConnectionUpdate {
    uint8_t connection;
    uint16_t interval_min;
    uint16_t interval_max;
    uint16_t latency;
    uint16_t timeout;
};

struct PACKED ConnectionUpdateResponse {
    uint8_t connection;
    uint16_t result;
};

Header getHeader(ConnectionUpdate *)
{
    return Header{0, 0, 0, sizeof(ConnectionUpdate), 3, 2};
}

struct PACKED ConnectionVersionUpdate {
    uint8_t connection;
};

struct PACKED ConnectionVersionUpdateResponse {
    uint8_t connection;
    uint16_t result;
};

Header getHeader(ConnectionVersionUpdate *)
{
    return Header{0, 0, 0, sizeof(ConnectionVersionUpdate), 3, 3};
}

template <int N>
struct PACKED ConnectionChannelMapGet {
    uint8_t connection;
};

template <int N>
struct PACKED ConnectionChannelMapGetResponse {
    uint8_t connection;
    uint8_t map[N];
};

template <int N>
Header getHeader(ConnectionChannelMapGet<N> *)
{
    return Header{0, 0, 0, sizeof(ConnectionChannelMapGet<N>), 3, 4};
}

template <int N>
struct PACKED ConnectionChannelMapSet {
    uint8_t connection;
    uint8_t map[N];
};

struct PACKED ConnectionChannelMapSetResponse {
    uint8_t connection;
    uint16_t result;
};

template <int N>
Header getHeader(ConnectionChannelMapSet<N> *)
{
    return Header{0, 0, 0, sizeof(ConnectionChannelMapSet<N>), 3, 5};
}

struct PACKED ConnectionFeaturesGet {
    uint8_t connection;
};

struct PACKED ConnectionFeaturesGetResponse {
    uint8_t connection;
    uint16_t result;
};

Header getHeader(ConnectionFeaturesGet *)
{
    return Header{0, 0, 0, sizeof(ConnectionFeaturesGet), 3, 6};
}

struct PACKED ConnectionGetStatus {
    uint8_t connection;
};

struct PACKED ConnectionGetStatusResponse {
    uint8_t connection;
};

Header getHeader(ConnectionGetStatus *)
{
    return Header{0, 0, 0, sizeof(ConnectionGetStatus), 3, 7};
}

template <int N>
struct PACKED ConnectionRawTx {
    uint8_t connection;
    uint8_t data[N];
};

struct PACKED ConnectionRawTxResponse {
    uint8_t connection;
};

template <int N>
Header getHeader(ConnectionRawTx<N> *)
{
    return Header{0, 0, 0, sizeof(ConnectionRawTx<N>), 3, 8};
}

struct PACKED ConnectionStatusEvent {
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
    uint8_t connection;
    uint8_t vers_nr;
    uint16_t comp_id;
    uint16_t sub_vers_nr;
};

template <int N>
struct PACKED ConnectionFeatureIndEvent {
    uint8_t connection;
    uint8_t features[N];
};

template <int N>
struct PACKED ConnectionRawRxEvent {
    uint8_t connection;
    uint8_t data[N];
};

struct PACKED ConnectionDisconnectedEvent {
    uint8_t connection;
    uint16_t reason;
};

template <int N>
struct PACKED AttclientFindByTypeValue {
    uint8_t connection;
    uint16_t start;
    uint16_t end;
    uint16_t uuid;
    uint8_t value[N];
};

struct PACKED AttclientFindByTypeValueResponse {
    uint8_t connection;
    uint16_t result;
};

template <int N>
Header getHeader(AttclientFindByTypeValue<N> *)
{
    return Header{0, 0, 0, sizeof(AttclientFindByTypeValue<N>), 4, 0};
}

template <int N>
struct PACKED AttclientReadByGroupType {
    uint8_t connection;
    uint16_t start;
    uint16_t end;
    uint8_t uuid[N];
};

struct PACKED AttclientReadByGroupTypeResponse {
    uint8_t connection;
    uint16_t result;
};

template <int N>
Header getHeader(AttclientReadByGroupType<N> *)
{
    return Header{0, 0, 0, sizeof(AttclientReadByGroupType<N>), 4, 1};
}

template <int N>
struct PACKED AttclientReadByType {
    uint8_t connection;
    uint16_t start;
    uint16_t end;
    uint8_t uuid[N];
};

struct PACKED AttclientReadByTypeResponse {
    uint8_t connection;
    uint16_t result;
};

template <int N>
Header getHeader(AttclientReadByType<N> *)
{
    return Header{0, 0, 0, sizeof(AttclientReadByType<N>), 4, 2};
}

struct PACKED AttclientFindInformation {
    uint8_t connection;
    uint16_t start;
    uint16_t end;
};

struct PACKED AttclientFindInformationResponse {
    uint8_t connection;
    uint16_t result;
};

Header getHeader(AttclientFindInformation *)
{
    return Header{0, 0, 0, sizeof(AttclientFindInformation), 4, 3};
}

struct PACKED AttclientReadByHandle {
    uint8_t connection;
    uint16_t chrhandle;
};

struct PACKED AttclientReadByHandleResponse {
    uint8_t connection;
    uint16_t result;
};

Header getHeader(AttclientReadByHandle *)
{
    return Header{0, 0, 0, sizeof(AttclientReadByHandle), 4, 4};
}

template <int N>
struct PACKED AttclientAttributeWrite {
    uint8_t connection;
    uint16_t atthandle;
    uint8_t data[N];
};

struct PACKED AttclientAttributeWriteResponse {
    uint8_t connection;
    uint16_t result;
};

template <int N>
Header getHeader(AttclientAttributeWrite<N> *)
{
    return Header{0, 0, 0, sizeof(AttclientAttributeWrite<N>), 4, 5};
}

template <int N>
struct PACKED AttclientWriteCommand {
    uint8_t connection;
    uint16_t atthandle;
    uint8_t data[N];
};

struct PACKED AttclientWriteCommandResponse {
    uint8_t connection;
    uint16_t result;
};

template <int N>
Header getHeader(AttclientWriteCommand<N> *)
{
    return Header{0, 0, 0, sizeof(AttclientWriteCommand<N>), 4, 6};
}

struct PACKED AttclientIndicateConfirm {
    uint8_t connection;
};

struct PACKED AttclientIndicateConfirmResponse {
    uint16_t result;
};

Header getHeader(AttclientIndicateConfirm *)
{
    return Header{0, 0, 0, sizeof(AttclientIndicateConfirm), 4, 7};
}

struct PACKED AttclientReadLong {
    uint8_t connection;
    uint16_t chrhandle;
};

struct PACKED AttclientReadLongResponse {
    uint8_t connection;
    uint16_t result;
};

Header getHeader(AttclientReadLong *)
{
    return Header{0, 0, 0, sizeof(AttclientReadLong), 4, 8};
}

template <int N>
struct PACKED AttclientPrepareWrite {
    uint8_t connection;
    uint16_t atthandle;
    uint16_t offset;
    uint8_t data[N];
};

struct PACKED AttclientPrepareWriteResponse {
    uint8_t connection;
    uint16_t result;
};

template <int N>
Header getHeader(AttclientPrepareWrite<N> *)
{
    return Header{0, 0, 0, sizeof(AttclientPrepareWrite<N>), 4, 9};
}

struct PACKED AttclientExecuteWrite {
    uint8_t connection;
    uint8_t commit;
};

struct PACKED AttclientExecuteWriteResponse {
    uint8_t connection;
    uint16_t result;
};

Header getHeader(AttclientExecuteWrite *)
{
    return Header{0, 0, 0, sizeof(AttclientExecuteWrite), 4, 10};
}

template <int N>
struct PACKED AttclientReadMultiple {
    uint8_t connection;
    uint8_t handles[N];
};

struct PACKED AttclientReadMultipleResponse {
    uint8_t connection;
    uint16_t result;
};

template <int N>
Header getHeader(AttclientReadMultiple<N> *)
{
    return Header{0, 0, 0, sizeof(AttclientReadMultiple<N>), 4, 11};
}

struct PACKED AttclientIndicatedEvent {
    uint8_t connection;
    uint16_t attrhandle;
};

struct PACKED AttclientProcedureCompletedEvent {
    uint8_t connection;
    uint16_t result;
    uint16_t chrhandle;
};

template <int N>
struct PACKED AttclientGroupFoundEvent {
    uint8_t connection;
    uint16_t start;
    uint16_t end;
    uint8_t uuid[N];
};

template <int N>
struct PACKED AttclientAttributeFoundEvent {
    uint8_t connection;
    uint16_t chrdecl;
    uint16_t value;
    uint8_t properties;
    uint8_t uuid[N];
};

template <int N>
struct PACKED AttclientFindInformationFoundEvent {
    uint8_t connection;
    uint16_t chrhandle;
    uint8_t uuid[N];
};

template <int N>
struct PACKED AttclientAttributeValueEvent {
    uint8_t connection;
    uint16_t atthandle;
    uint8_t type;
    uint8_t value[N];
};

template <int N>
struct PACKED AttclientReadMultipleResponseEvent {
    uint8_t connection;
    uint8_t handles[N];
};

struct PACKED SmEncryptStart {
    uint8_t handle;
    uint8_t bonding;
};

struct PACKED SmEncryptStartResponse {
    uint8_t handle;
    uint16_t result;
};

Header getHeader(SmEncryptStart *)
{
    return Header{0, 0, 0, sizeof(SmEncryptStart), 5, 0};
}

struct PACKED SmSetBondableMode {
    uint8_t bondable;
};

struct PACKED SmSetBondableModeResponse {
};

Header getHeader(SmSetBondableMode *)
{
    return Header{0, 0, 0, sizeof(SmSetBondableMode), 5, 1};
}

struct PACKED SmDeleteBonding {
    uint8_t handle;
};

struct PACKED SmDeleteBondingResponse {
    uint16_t result;
};

Header getHeader(SmDeleteBonding *)
{
    return Header{0, 0, 0, sizeof(SmDeleteBonding), 5, 2};
}

struct PACKED SmSetParameters {
    uint8_t mitm;
    uint8_t min_key_size;
    uint8_t io_capabilities;
};

struct PACKED SmSetParametersResponse {
};

Header getHeader(SmSetParameters *)
{
    return Header{0, 0, 0, sizeof(SmSetParameters), 5, 3};
}

struct PACKED SmPasskeyEntry {
    uint8_t handle;
    uint32_t passkey;
};

struct PACKED SmPasskeyEntryResponse {
    uint16_t result;
};

Header getHeader(SmPasskeyEntry *)
{
    return Header{0, 0, 0, sizeof(SmPasskeyEntry), 5, 4};
}

struct PACKED SmGetBonds {
};

struct PACKED SmGetBondsResponse {
    uint8_t bonds;
};

Header getHeader(SmGetBonds *)
{
    return Header{0, 0, 0, sizeof(SmGetBonds), 5, 5};
}

template <int N>
struct PACKED SmSetOobData {
    uint8_t oob[N];
};

struct PACKED SmSetOobDataResponse {
};

template <int N>
Header getHeader(SmSetOobData<N> *)
{
    return Header{0, 0, 0, sizeof(SmSetOobData<N>), 5, 6};
}

template <int N>
struct PACKED SmSmpDataEvent {
    uint8_t handle;
    uint8_t packet;
    uint8_t data[N];
};

struct PACKED SmBondingFailEvent {
    uint8_t handle;
    uint16_t result;
};

struct PACKED SmPasskeyDisplayEvent {
    uint8_t handle;
    uint32_t passkey;
};

struct PACKED SmPasskeyRequestEvent {
    uint8_t handle;
};

struct PACKED SmBondStatusEvent {
    uint8_t bond;
    uint8_t keysize;
    uint8_t mitm;
    uint8_t keys;
};

struct PACKED GapSetPrivacyFlags {
    uint8_t peripheral_privacy;
    uint8_t central_privacy;
};

struct PACKED GapSetPrivacyFlagsResponse {
};

Header getHeader(GapSetPrivacyFlags *)
{
    return Header{0, 0, 0, sizeof(GapSetPrivacyFlags), 6, 0};
}

struct PACKED GapSetMode {
    uint8_t discover;
    uint8_t connect;
};

struct PACKED GapSetModeResponse {
    uint16_t result;
};

Header getHeader(GapSetMode *)
{
    return Header{0, 0, 0, sizeof(GapSetMode), 6, 1};
}

struct PACKED GapDiscover {
    uint8_t mode;
};

struct PACKED GapDiscoverResponse {
    uint16_t result;
};

Header getHeader(GapDiscover *)
{
    return Header{0, 0, 0, sizeof(GapDiscover), 6, 2};
}

struct PACKED GapConnectDirect {
    uint8_t address[6];
    uint8_t addr_type;
    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t timeout;
    uint16_t latency;
};

struct PACKED GapConnectDirectResponse {
    uint16_t result;
    uint8_t connection_handle;
};

Header getHeader(GapConnectDirect *)
{
    return Header{0, 0, 0, sizeof(GapConnectDirect), 6, 3};
}

struct PACKED GapEndProcedure {
};

struct PACKED GapEndProcedureResponse {
    uint16_t result;
};

Header getHeader(GapEndProcedure *)
{
    return Header{0, 0, 0, sizeof(GapEndProcedure), 6, 4};
}

struct PACKED GapConnectSelective {
    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t timeout;
    uint16_t latency;
};

struct PACKED GapConnectSelectiveResponse {
    uint16_t result;
    uint8_t connection_handle;
};

Header getHeader(GapConnectSelective *)
{
    return Header{0, 0, 0, sizeof(GapConnectSelective), 6, 5};
}

struct PACKED GapSetFiltering {
    uint8_t scan_policy;
    uint8_t adv_policy;
    uint8_t scan_duplicate_filtering;
};

struct PACKED GapSetFilteringResponse {
    uint16_t result;
};

Header getHeader(GapSetFiltering *)
{
    return Header{0, 0, 0, sizeof(GapSetFiltering), 6, 6};
}

struct PACKED GapSetScanParameters {
    uint16_t scan_interval;
    uint16_t scan_window;
    uint8_t active;
};

struct PACKED GapSetScanParametersResponse {
    uint16_t result;
};

Header getHeader(GapSetScanParameters *)
{
    return Header{0, 0, 0, sizeof(GapSetScanParameters), 6, 7};
}

struct PACKED GapSetAdvParameters {
    uint16_t adv_interval_min;
    uint16_t adv_interval_max;
    uint8_t adv_channels;
};

struct PACKED GapSetAdvParametersResponse {
    uint16_t result;
};

Header getHeader(GapSetAdvParameters *)
{
    return Header{0, 0, 0, sizeof(GapSetAdvParameters), 6, 8};
}

template <int N>
struct PACKED GapSetAdvData {
    uint8_t set_scanrsp;
    uint8_t adv_data[N];
};

struct PACKED GapSetAdvDataResponse {
    uint16_t result;
};

template <int N>
Header getHeader(GapSetAdvData<N> *)
{
    return Header{0, 0, 0, sizeof(GapSetAdvData<N>), 6, 9};
}

struct PACKED GapSetDirectedConnectableMode {
    uint8_t address[6];
    uint8_t addr_type;
};

struct PACKED GapSetDirectedConnectableModeResponse {
    uint16_t result;
};

Header getHeader(GapSetDirectedConnectableMode *)
{
    return Header{0, 0, 0, sizeof(GapSetDirectedConnectableMode), 6, 10};
}

template <int N>
struct PACKED GapScanResponseEvent {
    int8_t rssi;
    uint8_t packet_type;
    uint8_t sender[6];
    uint8_t address_type;
    uint8_t bond;
    uint8_t data[N];
};

struct PACKED GapModeChangedEvent {
    uint8_t discover;
    uint8_t connect;
};

struct PACKED HardwareIoPortConfigIrq {
    uint8_t port;
    uint8_t enable_bits;
    uint8_t falling_edge;
};

struct PACKED HardwareIoPortConfigIrqResponse {
    uint16_t result;
};

Header getHeader(HardwareIoPortConfigIrq *)
{
    return Header{0, 0, 0, sizeof(HardwareIoPortConfigIrq), 7, 0};
}

struct PACKED HardwareSetSoftTimer {
    uint32_t time;
    uint8_t handle;
    uint8_t single_shot;
};

struct PACKED HardwareSetSoftTimerResponse {
    uint16_t result;
};

Header getHeader(HardwareSetSoftTimer *)
{
    return Header{0, 0, 0, sizeof(HardwareSetSoftTimer), 7, 1};
}

struct PACKED HardwareAdcRead {
    uint8_t input;
    uint8_t decimation;
    uint8_t reference_selection;
};

struct PACKED HardwareAdcReadResponse {
    uint16_t result;
};

Header getHeader(HardwareAdcRead *)
{
    return Header{0, 0, 0, sizeof(HardwareAdcRead), 7, 2};
}

struct PACKED HardwareIoPortConfigDirection {
    uint8_t port;
    uint8_t direction;
};

struct PACKED HardwareIoPortConfigDirectionResponse {
    uint16_t result;
};

Header getHeader(HardwareIoPortConfigDirection *)
{
    return Header{0, 0, 0, sizeof(HardwareIoPortConfigDirection), 7, 3};
}

struct PACKED HardwareIoPortConfigFunction {
    uint8_t port;
    uint8_t function;
};

struct PACKED HardwareIoPortConfigFunctionResponse {
    uint16_t result;
};

Header getHeader(HardwareIoPortConfigFunction *)
{
    return Header{0, 0, 0, sizeof(HardwareIoPortConfigFunction), 7, 4};
}

struct PACKED HardwareIoPortConfigPull {
    uint8_t port;
    uint8_t tristate_mask;
    uint8_t pull_up;
};

struct PACKED HardwareIoPortConfigPullResponse {
    uint16_t result;
};

Header getHeader(HardwareIoPortConfigPull *)
{
    return Header{0, 0, 0, sizeof(HardwareIoPortConfigPull), 7, 5};
}

struct PACKED HardwareIoPortWrite {
    uint8_t port;
    uint8_t mask;
    uint8_t data;
};

struct PACKED HardwareIoPortWriteResponse {
    uint16_t result;
};

Header getHeader(HardwareIoPortWrite *)
{
    return Header{0, 0, 0, sizeof(HardwareIoPortWrite), 7, 6};
}

struct PACKED HardwareIoPortRead {
    uint8_t port;
    uint8_t mask;
};

struct PACKED HardwareIoPortReadResponse {
    uint16_t result;
    uint8_t port;
    uint8_t data;
};

Header getHeader(HardwareIoPortRead *)
{
    return Header{0, 0, 0, sizeof(HardwareIoPortRead), 7, 7};
}

struct PACKED HardwareSpiConfig {
    uint8_t channel;
    uint8_t polarity;
    uint8_t phase;
    uint8_t bit_order;
    uint8_t baud_e;
    uint8_t baud_m;
};

struct PACKED HardwareSpiConfigResponse {
    uint16_t result;
};

Header getHeader(HardwareSpiConfig *)
{
    return Header{0, 0, 0, sizeof(HardwareSpiConfig), 7, 8};
}

template <int N>
struct PACKED HardwareSpiTransfer {
    uint8_t channel;
    uint8_t data[N];
};

template <int N>
struct PACKED HardwareSpiTransferResponse {
    uint16_t result;
    uint8_t channel;
    uint8_t data[N];
};

template <int N>
Header getHeader(HardwareSpiTransfer<N> *)
{
    return Header{0, 0, 0, sizeof(HardwareSpiTransfer<N>), 7, 9};
}

template <int N>
struct PACKED HardwareI2CRead {
    uint8_t address;
    uint8_t stop;
    uint8_t length;
};

template <int N>
struct PACKED HardwareI2CReadResponse {
    uint16_t result;
    uint8_t data[N];
};

template <int N>
Header getHeader(HardwareI2CRead<N> *)
{
    return Header{0, 0, 0, sizeof(HardwareI2CRead<N>), 7, 10};
}

template <int N>
struct PACKED HardwareI2CWrite {
    uint8_t address;
    uint8_t stop;
    uint8_t data[N];
};

struct PACKED HardwareI2CWriteResponse {
    uint8_t written;
};

template <int N>
Header getHeader(HardwareI2CWrite<N> *)
{
    return Header{0, 0, 0, sizeof(HardwareI2CWrite<N>), 7, 11};
}

struct PACKED HardwareSetTxpower {
    uint8_t power;
};

struct PACKED HardwareSetTxpowerResponse {
};

Header getHeader(HardwareSetTxpower *)
{
    return Header{0, 0, 0, sizeof(HardwareSetTxpower), 7, 12};
}

struct PACKED HardwareTimerComparator {
    uint8_t timer;
    uint8_t channel;
    uint8_t mode;
    uint16_t comparator_value;
};

struct PACKED HardwareTimerComparatorResponse {
    uint16_t result;
};

Header getHeader(HardwareTimerComparator *)
{
    return Header{0, 0, 0, sizeof(HardwareTimerComparator), 7, 13};
}

struct PACKED HardwareIoPortStatusEvent {
    uint32_t timestamp;
    uint8_t port;
    uint8_t irq;
    uint8_t state;
};

struct PACKED HardwareSoftTimerEvent {
    uint8_t handle;
};

struct PACKED HardwareAdcResultEvent {
    uint8_t input;
    int16_t value;
};

struct PACKED TestPhyTx {
    uint8_t channel;
    uint8_t length;
    uint8_t type;
};

struct PACKED TestPhyTxResponse {
};

Header getHeader(TestPhyTx *)
{
    return Header{0, 0, 0, sizeof(TestPhyTx), 8, 0};
}

struct PACKED TestPhyRx {
    uint8_t channel;
};

struct PACKED TestPhyRxResponse {
};

Header getHeader(TestPhyRx *)
{
    return Header{0, 0, 0, sizeof(TestPhyRx), 8, 1};
}

struct PACKED TestPhyEnd {
};

struct PACKED TestPhyEndResponse {
    uint16_t counter;
};

Header getHeader(TestPhyEnd *)
{
    return Header{0, 0, 0, sizeof(TestPhyEnd), 8, 2};
}

struct PACKED TestPhyReset {
};

struct PACKED TestPhyResetResponse {
};

Header getHeader(TestPhyReset *)
{
    return Header{0, 0, 0, sizeof(TestPhyReset), 8, 3};
}

template <int N>
struct PACKED TestGetChannelMap {
};

template <int N>
struct PACKED TestGetChannelMapResponse {
    uint8_t channel_map[N];
};

template <int N>
Header getHeader(TestGetChannelMap<N> *)
{
    return Header{0, 0, 0, sizeof(TestGetChannelMap<N>), 8, 4};
}

template <int N>
struct PACKED TestDebug {
    uint8_t input[N];
};

template <int N>
struct PACKED TestDebugResponse {
    uint8_t output[N];
};

template <int N>
Header getHeader(TestDebug<N> *)
{
    return Header{0, 0, 0, sizeof(TestDebug<N>), 8, 5};
}

template <typename T>
struct PACKED Command {
    Header header;
    T payload;
};

#endif // BLEAPI_H

