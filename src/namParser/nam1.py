import json
import os
import sys

def generate_header(nam_filename, output_header):
    # Read the .nam file
    with open(nam_filename, "r") as file:
        nam_data = json.load(file)
    base_name = os.path.splitext(input_filename)[0]
    # Extract fields from the .nam data
    version = nam_data["version"]
    architecture = nam_data["architecture"]
    config = json.dumps(nam_data["config"], indent=4).replace("\n", "\\n").replace("\"", "\\\"")
    metadata = json.dumps(nam_data["metadata"], indent=4).replace("\n", "\\n").replace("\"", "\\\"")
    weights = nam_data["weights"]
    sample_rate = nam_data["sample_rate"]

    # Generate the .h file content
    with open(output_header, "w") as header_file:
        
        header_file.write("#ifndef DSPDATA_H\n")
        header_file.write("#define DSPDATA_H\n")
        #header_file.write("#pragma once\n\n")
        header_file.write("#include <string>\n")
        header_file.write("#include <vector>\n")
        header_file.write("#include <nlohmann/json.hpp>\n")
        header_file.write("#include \"../../NeuralAmpModelerCore/NAM/dsp.h\"\n\n")
        header_file.write("// Automatically generated header file\n\n")
        
        # Create an instance of the struct
        header_file.write(f"inline nam::dspData {base_name} = {{\n")
        header_file.write(f"    \"{version}\", // version\n")
        header_file.write(f"    \"{architecture}\", // architecture\n")
        header_file.write(f"    nlohmann::json::parse(\"{config}\"), // config\n")
        header_file.write(f"    nlohmann::json::parse(\"{metadata}\"), // metadata\n")
        
        # Write the weights array
        header_file.write("    {")
        header_file.write(", ".join(f"{w}f" for w in weights))
        header_file.write("}, // weights\n")
        
        # Write the sample rate
        header_file.write(f"    {sample_rate} // expected_sample_rate\n")
        header_file.write("};\n")
        header_file.write("#endif DSPDATA_H\n")

if __name__ == "__main__":
    # Convert .nam to .h
    if len(sys.argv) != 2:
        print("Usage: python script.py <input_filename.nam>")
        sys.exit(1)

    input_filename = sys.argv[1]
    
    # Validate input file
    if not input_filename.endswith(".nam") or not os.path.isfile(input_filename):
        print("Error: Input file must be a valid .nam file.")
        sys.exit(1)

    # Generate output filename
    output_filename = f"{os.path.splitext(input_filename)[0]}.h"

    
    # nam_file = "example.nam"  # Input .nam file
    # header_file = "dspData.h"  # Output header file
    generate_header(input_filename, output_filename)
    print(f"Header file generated: {output_filename}")