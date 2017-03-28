# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

import xml.etree.ElementTree as ET
import textwrap

"""
BGAPI packet structure (as of 2012-11-07):
  Byte 0:
        [7] - 1 bit, Message Type (MT)         0 = cmd/Response, 1 = Event
      [6:3] - 4 bits, Technology Type (TT)     0000 = Bluetooth 4.0 single mode,
                                               0001 = Wi-Fi
      [2:0] - 3 bits, Length High (LH)         Payload length (high bits)
  Byte 1:     8 bits, Length Low (LL)          Payload length (low bits)
  Byte 2:     8 bits, Class ID (CID)           cmd class ID
  Byte 3:     8 bits, cmd ID (CMD)             cmd ID
  Bytes 4-n:  0 - 2048 Bytes, Payload (PL)     Up to 2048 bytes of payload
"""


def type_mapping(type):
    return {
        'int8': 'std::int8_t {}',
        'uint8': 'std::uint8_t {}',
        'int16': 'std::int16_t {}',
        'uint16': 'std::uint16_t {}',
        'uint32': 'std::uint32_t {}',
        'bd_addr': 'std::uint8_t {}[6]',
        'uint8array': 'std::uint8_t {}[N]'
    }[type]


def generate_file_header(f):
    f.write(textwrap.dedent('''\
    /* This Source Code Form is subject to the terms of the Mozilla Public
     * License, v. 2.0. If a copy of the MPL was not distributed with this
     * file, You can obtain one at http://mozilla.org/MPL/2.0/. */\n
    '''))

    f.write('#pragma once\n')
    f.write('#ifndef BLEAPI_H\n')
    f.write('#define BLEAPI_H\n\n')
    f.write('#include <cinttypes>\n')
    f.write('#define PACKED __attribute__((__packed__))\n')


def generate_file_footer(f):
    f.write('#endif // BLEAPI_H')


def camel_case(s):
    return s.title().replace('_', '')

def generate_struct(f, cls_index, cmd_index, params, suffix='', variable_size=False):
    struct_name = cls_name.title() + camel_case(cmd_name.title())

    if variable_size:
        f.write(f'template <int N>\n')

    f.write(f'struct PACKED {struct_name}{suffix} {{\n')
    f.write(f'    enum {{ cls = {cls_index}, cmd = {cmd_index} }};\n');

    length_sig = 'std::uint8_t length'

    for param in params:
        param_name = param.attrib['name']
        param_type = param.attrib['type']
        param_sig = type_mapping(param_type).format(param_name)

        if param_type == 'uint8array':
            f.write(f'    {length_sig};\n')
        f.write(f'    {param_sig};\n')
    f.write('};\n\n')

    if variable_size:
        f.write(f'template <>\n')
        f.write(f'struct PACKED {struct_name}{suffix}<0> {{\n')
        f.write(f'    enum {{ cls = {cls_index}, cmd = {cmd_index} }};\n');

        for param in params:
            param_name = param.attrib['name']
            param_type = param.attrib['type']
            param_sig = type_mapping(param_type).format(param_name)

            if param_type == 'uint8array':
                f.write(f'    {length_sig};\n')
            else:
                f.write(f'    {param_sig};\n')
        f.write('};\n\n')

if __name__ == '__main__':
    tree = ET.parse('bleapi.xml')
    root = tree.getroot()

    f = open('bleapi.h', 'w')
    generate_file_header(f)

    f.write(textwrap.dedent('''
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
        };\n
        '''))

    for cls in root.findall("class"):
        cls_name = cls.attrib['name']
        cls_index = cls.attrib['index']

        for cmd in cls.findall('command'):
            cmd_index = cmd.attrib['index']
            cmd_name = cmd.attrib['name']

            generate_struct(f, cls_index, cmd_index, cmd.findall('params/param'),
                            variable_size=cmd.findall(".//params/param[@type='uint8array']"))

            generate_struct(f, cls_index, cmd_index, cmd.findall('returns/param'), suffix='Response',
                            variable_size=cmd.findall(".//returns/param[@type='uint8array']"))

        for cmd in cls.findall('event'):
            cmd_index = cmd.attrib['index']
            cmd_name = cmd.attrib['name']

            generate_struct(f, cls_index, cmd_index, cmd.findall('params/param'), suffix='Event',
                            variable_size=cmd.findall(".//param[@type='uint8array']"))

        for enums in cls.findall('enums'):
            enums_name = cls_name.title() + camel_case(enums.attrib['name']) + 'Enum'
            f.write(f'namespace {enums_name} {{\n')
            f.write('enum {\n')

            enum_statements = []
            for enum in enums.findall('enum'):
                enum_name = camel_case(enum.attrib['name'])
                enum_value = enum.attrib['value']

                enum_statements.append(f'    {enum_name} = {enum_value}')

            f.write(',\n'.join(enum_statements))
            f.write('\n};\n}\n\n')

    generate_file_footer(f)
    f.close()
