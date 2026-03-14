if(NOT SOURCE_DIR)
    set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# https://stackoverflow.com/a/27206982
function(create_resources dir output header_guard namespace)
    # Create output file
    file(WRITE ${output} "#ifndef ${header_guard}\n#define ${header_guard}\n\n#pragma clang diagostic push\n#pragma clang diagnostic ignored \"-Wc23-extensions\"\n\nnamespace ${namespace}\n{")
    # Collect input files
    file(GLOB bins ${dir}/*)
    # Iterate through input files
    foreach(bin ${bins})
        # Get short filename
        string(REGEX MATCH "([^/]+)$" file_name ${bin})
        string(REGEX REPLACE "\\..+" "" filename ${file_name})
        # Replace filename spaces & extension separator for C compatibility
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
        if(WIN32)
            # Read hex data from file
            file(READ ${bin} filedata HEX)
            # Convert hex data for C compatibility
            string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
            # Append data to output file
            file(APPEND ${output} "\n\tconstinit const unsigned char ${filename}[]{${filedata}};\n")
        else()
            file(APPEND ${output} "\n\tconstinit const unsigned char ${filename}[]{\n\t\t#embed \"${dir}/${file_name}\"\n\t};\n")
        endif()
    endforeach()
    file(APPEND ${output} "}\n\n#endif // ${header_guard}\n")
endfunction()

create_resources(${SOURCE_DIR}/models models.hpp ROSE_TINTED_MODELS_H Models)
