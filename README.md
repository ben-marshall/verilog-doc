
# Verilog Doc

[![Documentation](https://codedocs.xyz/ben-marshall/verilog-doc.svg)](https://codedocs.xyz/ben-marshall/verilog-doc/)

My ideas on creating an automatic documentation generation tool for Verilog,
using or abusing Doxygen to get there.

---

ToDo List:

- Module Page JSON Emission
 - Nets
 - Functions / Tasks
 - Instantiations
 - Processes / Blocks
- Module Page JSON Rendering
- HTML "Distribution" creation

---

### Main Sections:

- Home Page
- File List
- Module List
- Module Hierarchy
 - Module Pages

### Program Stages

1. Load Command Line Arguments
2. Load configuration file if any.
3. Parse input files.
4. Organise parsed data structures.
5. Generate Pages
 - File List
 - Module List
 - Module Hierarchy
 - Module Pages
6. Cleanup
