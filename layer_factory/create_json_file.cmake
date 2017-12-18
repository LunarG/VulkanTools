#
# Modify framework json file for each resultant factory layer DLL
#
file(READ ${srcfile} json_file_template)
string(REPLACE "layer_factory" ${target} target_json_file ${json_file_template})
file(TO_NATIVE_PATH ${dstpath}/VkLayer_${target}.json dst_json)
file(WRITE ${dst_json} ${target_json_file})
