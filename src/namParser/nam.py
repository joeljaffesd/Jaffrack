import json

def generate_header(nam_filename, output_header):
    # Read the .nam file
    with open(nam_filename, "r") as file:
        nam_data = json.load(file)
    
    # Extract fields from the .nam data
    version = nam_data["version"]
    architecture = nam_data["architecture"]
    config = json.dumps(nam_data["config"], indent=4).replace("\n", "\\n").replace("\"", "\\\"")
    metadata = json.dumps(nam_data["metadata"], indent=4).replace("\n", "\\n").replace("\"", "\\\"")
    weights = nam_data["weights"]
    sample_rate = nam_data["sample_rate"]

    # Generate the .h file content
    with open(output_header, "w") as header_file:
        header_file.write("#pragma once\n\n")
        header_file.write("#include <string>\n")
        header_file.write("#include <vector>\n")
        header_file.write("#include <nlohmann/json.hpp>\n\n")
        header_file.write("// Automatically generated header file\n\n")
        header_file.write("struct dspData {\n")
        header_file.write("    std::string version;\n")
        header_file.write("    std::string architecture;\n")
        header_file.write("    nlohmann::json config;\n")
        header_file.write("    nlohmann::json metadata;\n")
        header_file.write("    std::vector<float> weights;\n")
        header_file.write("    double expected_sample_rate;\n")
        header_file.write("};\n\n")
        
        # Create an instance of the struct
        header_file.write("inline dspData dspDataInstance = {\n")
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

if __name__ == "__main__":
    # Convert .nam to .h
    nam_file = "microMarshall.nam"  # Input .nam file
    header_file = "mDspData.h"  # Output header file
    generate_header(nam_file, header_file)
    print(f"Header file generated: {header_file}")