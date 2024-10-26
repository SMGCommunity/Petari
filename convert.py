def convert_entry(entry):
    obj_name = entry.split("[")[1].split(".o")[0]
    new_path = f"JSystem/JUtility/{obj_name}.cpp"
    return f'Object(NonMatching, "{new_path}"),'

def process_file(input_filename, output_filename):
    with open(input_filename, 'r') as infile, open(output_filename, 'w') as outfile:
        for line in infile:
            converted_line = convert_entry(line.strip())
            outfile.write(converted_line + '\n')

process_file('input.txt', 'output.txt')
