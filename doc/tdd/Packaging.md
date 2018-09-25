# Packaging

## Node
A node is a binary that conforms to the Node's API

### API
#### CLI arguments
--show-extensions : Display all the extensions this packager supports
Each extension is displayed on a single line
exemple:

$ smpackager --show-extensions
> 3D
> 3DS
> 3MF
> AC
> AC3D
> ACC
> ...
> OBJ

--pack: Generate packaged data from input_files
For each generated entry, an id must be written on a newline of stdout
The id is linked with an input_file (keep the same sequence)

$ smpackager --pack -o output_dir input_file input_file2 input_file3
> identifier_1 status (id for input_file)
> identifier_2 status (id for input_file2)
> identifier_3 status (id for input_file3)
