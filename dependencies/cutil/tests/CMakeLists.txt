link_libraries(cutil)

add_executable(listTests list.c)
add_test(NAME ListTests COMMAND listTests)

add_executable(hashTableTests hashTable.c)
add_test(NAME HashTableTests COMMAND hashTableTests)

add_executable(filesystemTests filesystem.c)
add_test(NAME FilesystemTests COMMAND filesystemTests)

add_executable(mathTests math.c)
add_test(NAME MathTests COMMAND mathTests)