import json
import sys
from pathlib import Path

def parse_nam_file(nam_file_path):
    # Read and parse the .nam file
    with open(nam_file_path, "r") as f:
        data = json.load(f)

    # Extract required fields
    dsp_data = {
        "version": data.get("version", ""),
        "architecture": data.get("architecture", ""),
        "config": json.dumps(data.get("config", {})),
        "metadata": json.dumps(data.get("metadata", {})),
        "weights": data.get("weights", []),
        "expected_sample_rate": data.get("sample_rate", -1.0)
    }

    return dsp_data


def generate_header_file(dsp_data, output_file):
    # Generate the header content
    header_content = f"""\
#ifndef DSP_DATA_H
#define DSP_DATA_H

#include <vector>
#include <string>

struct dspData {{
    std::string version = "{dsp_data['version']}";
    std::string architecture = "{dsp_data['architecture']}";
    std::string config = R"({dsp_data['config']})";
    std::string metadata = R"({dsp_data['metadata']})";
    std::vector<float> weights = {{{", ".join(map(str, dsp_data['weights']))}}};
    float expected_sample_rate = {dsp_data['expected_sample_rate']};
}};

#endif // DSP_DATA_H
"""

    # Write the content to the output file
    with open(output_file, "w") as f:
        f.write(header_content)

    print(f"Header file generated: {output_file}")


def main():
    if len(sys.argv) != 3:
        print("Usage: python generate_header.py <input.nam> <output.h>")
        return

    nam_file = Path(sys.argv[1])
    output_file = Path(sys.argv[2])

    if not nam_file.exists():
        print(f"Error: {nam_file} does not exist.")
        return

    dsp_data = parse_nam_file(nam_file)
    generate_header_file(dsp_data, output_file)


if __name__ == "__main__":
    main()