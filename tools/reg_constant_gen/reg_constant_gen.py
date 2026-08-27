# To use this tool:
# Ensure terminal is running tool in the same directory as the tools location
# Typing the terminals path with not work. Ex. "Python3 ./tools/reg_constant_reg.py"
# You will then be prompted by the script for constant format (macro or enum)
# The script will output a text file to the "out directory"

import xml.etree.ElementTree as ET
import re
import os

tree = ET.parse('STM32F407.svd')
root = tree.getroot()

dir_name = "out"
output_format = "None"
generic_peri_name = "Error"
output_file = generic_peri_name + ".txt"

def find_peri():
    global generic_peri_name 
    global output_file 
    global output_format
    found_peri = False
    peri_derived = root.find("")

    print("What format would you like:")
    print("1. Enumerations")
    print("2. Macro")
    user_input = input()
    if user_input != "1" and user_input != "2":
        print("Unknown option. Ending script")
        exit()
    elif(user_input == "1"):
       output_format = "enum" 
    elif(user_input == "2"):
       output_format = "macro" 

    print("What peripheral would you like")
    user_peri = input()
    user_peri = user_peri.upper()

    for peri_element in root.iter('peripheral'):
        peri_name = peri_element.find('name').text

        # If peripheral found 
        if user_peri == peri_name:
            print("Enter generic name for peripheral:", peri_name)
            generic_peri_name= input()
            found_peri = True
            output_file = dir_name + "/" + generic_peri_name + ".txt"
            peri_derived = peri_element.get('derivedFrom')
            if (str(peri_derived) != "None"):
                print("Getting derived peripheral...", peri_derived)
                # If derived, get the derived peripheral register info instead
                for derived_peri_element in root.iter('peripheral'):
                    if peri_derived == derived_peri_element.find('name').text:
                        return derived_peri_element
            else:
                return peri_element

    if found_peri == False:
        print("Peripheral not found. Ending script.")
        exit()

def bit_pos_out(reg, generic_peri_name):
    with open(output_file, "a") as f:
        if output_format == "enum":
            print("typedef enum {", file=f)

        reg_name = str(reg.find('name').text).upper()
        max_offset_str_width = max(len(str(field.find('bitOffset').text)) for field in reg.iter('field'))
        max_field_width = max(len(field.find('name').text) for field in reg.iter('field'))
        max_field_width += len(generic_peri_name) + len(reg_name) + 2 + len("_POS")
        for field in reg.iter('field'):

            field_name = str(field.find('name').text).upper()
            field_offset = field.find('bitOffset').text
            field_str = generic_peri_name.upper() + "_" + reg_name + "_" + field_name + "_POS"
            field_desc = field.find('description').text
            field_desc = str(field_desc).replace("\n", "")
            field_desc = re.sub(" +", " ", field_desc)
            
            if output_format == "enum":
                print(f"    {field_str:{max_field_width}} = {field_offset + ",":{max_offset_str_width + 4}} // {field_desc}" , file=f)
            elif output_format == "macro":
                print(f"#define {field_str:{max_field_width}} {field_offset:{max_offset_str_width + 4}} // {field_desc}" , file=f)

        if output_format == "enum":
            print(f"}} {generic_peri_name.lower()}_{str(reg_name).lower()}_pos_e;", file=f)
        print(file=f)

def convert_width_to_string(width):
    string = "0b"
    string += "1" * int(width)
    return string

def bit_mask_out(reg, generic_peri_name):
    with open(output_file, "a") as f:
        if output_format == "enum":
            print("typedef enum {", file=f)

        reg_name = str(reg.find('name').text).upper()
        max_bitwidth_width = max(len(str(convert_width_to_string(field.find('bitWidth').text))) for field in reg.iter('field'))
        max_field_width = max(len(field.find('name').text) for field in reg.iter('field'))
        max_field_width += len(generic_peri_name) + len(reg_name) + 2 + len("_MASK")
        for field in reg.iter('field'):

            field_name = str(field.find('name').text).upper()
            field_width = convert_width_to_string(field.find('bitWidth').text)
            field_str = generic_peri_name.upper() + "_" + reg_name + "_" + field_name + "_MASK"
            num_of_bits = field.find('bitWidth').text

            if output_format == "enum":
                print(f"    {field_str:{max_field_width}} = {field_width + ",":{max_bitwidth_width + 4}} // {num_of_bits} bit(s)" , file=f)
            elif output_format == "macro":
                print(f"#define {field_str:{max_field_width}} {field_width:{max_bitwidth_width + 4}} // {num_of_bits} bit(s)" , file=f)

        if output_format == "enum":
            print(f"}} {generic_peri_name.lower()}_{str(reg_name).lower()}_mask_e;", file=f)
        print(file=f)

def bit_out(reg, generic_peri_name):
    with open(output_file, "a") as f:
        if output_format == "enum":
            print("typedef enum {", file=f)

        reg_name = str(reg.find('name').text).upper()
        max_pos_str_width = max(len(field.find('name').text) for field in reg.iter('field'))
        max_pos_str_width += len(generic_peri_name) + len(reg_name) + 2 + len("_POS")
        max_field_width = max(len(field.find('name').text) for field in reg.iter('field'))
        max_field_width += len(generic_peri_name) + len(reg_name) + 2 

        for field in reg.iter('field'):
            field_name = str(field.find('name').text).upper()
            field_name = generic_peri_name.upper() + "_" + reg_name + "_" + field_name
            field_pos = field_name + "_POS"
            field_desc = field.find('description').text
            field_desc = str(field_desc).replace("\n", "")
            field_desc = re.sub(" +", " ", field_desc)

            if output_format == "enum":
                print(f"    {field_name:{max_field_width}} = (1 << {field_pos + "),":{max_pos_str_width + 4}} // {field_desc}" , file=f)
            elif output_format == "macro":
                print(f"#define {field_name:{max_field_width}} (1 << {field_pos + ")":{max_pos_str_width + 4}} // {field_desc}" , file=f)

        if output_format == "enum":
            print(f"}} {generic_peri_name.lower()}_{str(reg_name).lower()}_e;", file=f)
        print(file=f)

def reg_struct_out(peri, generic_peri_name):
    with open(output_file, "a") as f:
        print("typedef __vo struct {", file=f)

        previous_offset = 0
        reserved_num = 0
        reserved_str = "reserved_"
        found_offset_list = []

        max_name_width = max(len(str(reg.find('name').text)) for reg in peri.iter('register'))
        max_name_width = max(max_name_width, len("reserved_xx"))
        max_desc_width = max(len(re.sub(" +", " ",str(reg.find('description').text))) for reg in peri.iter('register'))
        max_desc_width = max(max_desc_width, len("Reserved section #"))

        for reg in peri.iter('register'):
            name = reg.find('name').text
            offset = reg.find('addressOffset').text
            desc = reg.find('description').text
            desc = str(desc).replace("\n", "")
            desc = re.sub(" +", " ", desc)

            # Get rid of appended _x and (x) as these are generally used to specify
            # Alternate versions of a register
            name = str(name).split("_")[0]
            desc = str(desc).split("(")[0]

            # Break if negative, happens when wrapping around registers
            if ((int(offset, base=16) - previous_offset) < 0):
                break

            # Don't print repeated offsets
            if (int(offset, base=16) == previous_offset and int(offset, base=16) != 0):
                continue

            # Check for any offset gaps
            while (int(offset, base=16) - previous_offset) > 4 and offset != 0:
                previous_offset += 4
                reserved_num += 1
                list_size = len(found_offset_list)

                # Print any out of order registers
                for reg_find in peri.iter('register'):
                    desc_find = reg_find.find('description').text
                    if reg_find.find('addressOffset').text == str(hex(previous_offset)):
                        print(f"    uint32_t {reg_find.find('name').text + ";":{max_name_width + 4}} // {desc_find:{max_desc_width + 4}} Offset: 0x{(str(hex(previous_offset)).upper()).split("X")[1]}", file=f)
                        found_offset_list.append(reg_find.find('addressOffset').text)
                        break

                # Print reserved address block
                if len(found_offset_list) == list_size:
                    print(f"    uint32_t {reserved_str + str(reserved_num) + ";":{max_name_width + 4}} // Reserved 0x{(str(hex(previous_offset)).upper()).split("X")[1]}", file=f)

            # Print normally (not already repeated)
            if not (offset in found_offset_list):
                print(f"    uint32_t {name + ";":{max_name_width + 4}} // {desc:{max_desc_width + 4}} Offset: {offset}", file=f)

            previous_offset = int(offset, base=16)

        print("}", generic_peri_name.lower() + "_reg_def" + ";", file=f)
        print(file=f)

dir_list = os.listdir(os.getcwd())

if not(any (dir_name in file_dir for file_dir in dir_list)):
    os.mkdir(dir_name)


peri_element = find_peri()

open(output_file, 'w').close()

reg_struct_out(peri_element, generic_peri_name)

for reg in peri_element.iter('register'):
    bit_pos_out(reg, generic_peri_name)
    bit_out(reg, generic_peri_name)
    bit_mask_out(reg, generic_peri_name)

print(f"File \"{output_file}\" generated.")

