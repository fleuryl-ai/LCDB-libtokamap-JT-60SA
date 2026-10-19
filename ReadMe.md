# UDA LCDB Plugin Sample

UDA plugin for accessing LCDB through LibTokaMap.

This plugin provides:

- LCDB access through UDA
- LibTokaMap integration
- Shot-number based partitioning
- DD version based partitioning

## Build
Load the required modules:
```bash
module load UDA/2.9.3-GCC-14.3.0
```
or
```bash
module load UDA/2.9.3-GCC-13.2.0
```

Configure:
Check CMakeLists.txt and fix path.
```bash
cmake -B build \
    -DCMAKE_BUILD_TYPE=Debug
```

Build:
```bash
cmake --build build -j
```

## UDA Usage

```bash
uda_cli \
-p 56565 \
"LCDB::get(
 mapping='test',
 path='lcdb/xexpdata',
 datatype='8',
 rank='3',
 root='/path/to/data',
 shot='75000',
 category='test.s001'
)"
```

## Unit test

Manually write the calls to the test functions to be executed in `test_main.cpp`. 
No testing libraries or similar tools are used.

### Run Tests 
```bash
./build/lcdb_tests
```
```text 
[RUN ] ... 
[ OK ] ...
```

## Configuration

The plugin reads mapping definitions through
`config.toml`.

Example:

```toml
mapping_directory = "mappings"
schemas_directory = "schemas"
cache_enabled = false
```

## Format

Format code before commit.
```bash
git ls-files '*.cpp' '*.hpp' '*.h' | xargs clang-format -i
```

---

## Developer Notes

### Error Stack

UDA uses a global error stack.

Tests should call:

```cpp
freeIdamErrorStack(...)
initUdaErrorStack()
```

### DataType
When generating a mapping handler and calling the `map` function, 
please note that the `std::type_index` shown in the libtokamap sample code is not supported in the actual source code.

Sample code:
```cpp
TypedDataArray map(const std::string& experiment,
                   const std::string& path,
                   std::type_index data_type,
                   int rank,
                   const nlohmann::json& extra_attributes);
```

It is necessary to use `libtokamap::DataType`.
```cpp
libtokamap::TypedDataArray libtokamap::MappingHandler::map(const ExperimentName& experiment, const std::string& path,
                                                           libtokamap::DataType data_type, int rank,
                                                           const nlohmann::json& extra_attributes)
```

### String Support

String values are internally represented as
DataType::Int8.

String arrays are represented as
two-dimensional Int8 arrays.


### Partition
```text
string attribute
 → exact

numeric attribute
 → max_below
 → min_above
 → closest
```

