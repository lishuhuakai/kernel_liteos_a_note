import argparse
import os
import sys
import json
import ccsyspath
import time
import clang.cindex
from clang.cindex import Cursor
from clang.cindex import CursorKind
from pathlib import Path
from clang.cindex import TranslationUnit


def get_args():
    '''
    Get and parse the parameters from the console
    '''
    parse = argparse.ArgumentParser(prog='python3.8 compare.py')
    parse.add_argument('-l',
                       type=str, 
                       required=True,
                       help='input lib file path')
    parse.add_argument('-i',
                       type=str,
                       required=True,
                       help='input head file path')
    parse.add_argument('-m', type=str, help='input macros file path')
    parse.add_argument('-b', type=str, help='input blacklist file path')
    parse.add_argument('-o', type=str, help='result file output path')
    args = parse.parse_args()
    paramers_value = vars(args)
    global INPUT_HEAD_PATH
    INPUT_HEAD_PATH = paramers_value['i']
    global INPUT_LIB_PATH
    INPUT_LIB_PATH = paramers_value['l']
    global INPUT_MACROS_PATH
    INPUT_MACROS_PATH = paramers_value['m']
    INPUT_MACROS_PATH = '' if INPUT_MACROS_PATH == None else INPUT_MACROS_PATH
    global INPUT_BLACK_PATH
    INPUT_BLACK_PATH = paramers_value['b']
    INPUT_BLACK_PATH = '' if INPUT_BLACK_PATH == None else INPUT_BLACK_PATH
    global OUTPUT_RESULT_PATH
    OUTPUT_RESULT_PATH = paramers_value['o']
    OUTPUT_RESULT_PATH = '' if OUTPUT_RESULT_PATH == None else OUTPUT_RESULT_PATH
    check_parameters()


def check_parameters():
    '''
    Check whether the obtained parameters are correct
    '''
    my_file = Path(INPUT_LIB_PATH)
    if not my_file.is_file():
        print('please input correct lib file path')
        exit()
    my_file = Path(INPUT_HEAD_PATH)
    if not my_file.is_dir():
        print('please input correct head file path')
        exit()
    global INPUT_BLACK_PATH
    if not INPUT_BLACK_PATH == '':
        my_file = Path(INPUT_BLACK_PATH)
        if not my_file.is_file():
            print('warring:input correct blacklist file path is error')
            INPUT_BLACK_PATH = ''
    global INPUT_MACROS_PATH
    if not INPUT_MACROS_PATH == '':
        my_file = Path(INPUT_MACROS_PATH)
        if not my_file.is_file():
            print('warring:input correct macros file path is error')
            INPUT_MACROS_PATH = ''
    global OUTPUT_RESULT_PATH
    if not OUTPUT_RESULT_PATH == '':
        my_file = Path(OUTPUT_RESULT_PATH)
        if not my_file.is_dir():
            print('warring:input correct output file path is error')
            OUTPUT_RESULT_PATH = ''


def os_popen(stmt):
    '''
    Get the result of execution according to command parameter
    '''
    re = os.popen(stmt).readlines()
    result = []
    for i in range(0, len(re)):
        res = re[i].strip('\n')
        result.append(res)
    return result


def get_info_from_file(file_path):
    '''
    Get the list of contents in the file based on the path parameter
    '''
    temp_file_list = []
    with open(file_path) as file_object:
        for line in file_object:
            temp_file_list.append(line.rstrip())
    return temp_file_list


def get_lib_strs():
    '''
    Get a list of library file symbols
    '''
    temp_lib_name_list = os_popen(
        'nm -D ' + INPUT_LIB_PATH +
        '| grep -Ev " U " | grep -Ev " W " | grep -Ev " D " | grep -Ev " V " | grep -Ev " w " | awk \'{print $3}\' | xargs c++filt'
    )
    if len(temp_lib_name_list) == 0:
        print("canot find lib file  error")
        exit()
    global LIB_FILE_NAME_LIST
    for i in temp_lib_name_list:
        LIB_FILE_NAME_LIST.append(sub_str_name(i))
    LIB_FILE_NAME_LIST = list(set(LIB_FILE_NAME_LIST))
    LIB_FILE_NAME_LIST.sort()


def get_permission_num(permissions):
    '''
    Get the number of arguments of c++ function
    '''
    if not permissions.find('()') == -1:
        return 0
    else:
        count_premission = 1
        current_sym_num = 0
        for i in permissions:
            if i == '<':
                current_sym_num = current_sym_num + 1
            if i == '>':
                current_sym_num = current_sym_num - 1
            if i == ',' and current_sym_num == 0:
                count_premission = count_premission + 1
        return count_premission


def sub_str_name(iteam):
    '''
    Handling redundant information in library file symbol table
    '''
    if not iteam.find('non-virtual thunk to ') == -1:
        iteam = iteam.replace('non-virtual thunk to ', '')
    if not iteam.find('virtual thunk to ') == -1:
        iteam = iteam.replace('virtual thunk to ', '')
    if iteam.find('(') == -1:
        return iteam
    else:
        return iteam[:iteam.index('(')] + '@' + str(get_permission_num(iteam))


def get_head_strs():
    '''
    Get a list of header file symbols
    '''
    head_file_name_list = os_popen('find ' + INPUT_HEAD_PATH + ' -name "*.h"')
    if len(head_file_name_list) == 0:
        print('canot find head file error')
        exit()
    compile_args = ['-x', 'c++']
    marcros_list = []
    if not INPUT_MACROS_PATH == '':
        temp_macros = get_info_from_file(INPUT_MACROS_PATH)
        for i in temp_macros:
            marcros_list.append('-D' + i)
    for i in head_file_name_list:
        global CURRENT_FILE_TYPE
        CURRENT_FILE_TYPE = 0
        index = clang.cindex.Index.create()
        parser = index.parse(i, args=compile_args)
        cursor = parser.cursor
        traverse(cursor)
        if CURRENT_FILE_TYPE == 0:
            index_temp = clang.cindex.Index.create()
            syspath = ccsyspath.system_include_paths('clang')
            incargs = [b'-I' + inc for inc in syspath]
            incargs.append('-I'+INPUT_HEAD_PATH)
            parser_temp = index_temp.parse(i, args=marcros_list + incargs)
            cursor_temp = parser_temp.cursor
            traverse_c(cursor_temp, 0)
        else:
            index_temp = clang.cindex.Index.create()
            syspath = ccsyspath.system_include_paths('clang++')
            incargs = [b'-I' + inc for inc in syspath]
            incargs.append('-I'+INPUT_HEAD_PATH)
            parser_temp = index_temp.parse(i,
                                           args=marcros_list + compile_args +
                                           incargs)
            cursor_temp = parser_temp.cursor
            traverse_cpp(cursor_temp, '')
    global HEAD_FILE_NAME_LIST
    HEAD_FILE_NAME_LIST = list(set(HEAD_FILE_NAME_LIST))
    HEAD_FILE_NAME_LIST.sort()


def traverse_c(node, depth):
    '''
    Recursively obtain the symbol list from the c language header file and store it in HEAD_FILE_NAME_LIST
    '''
    global HEAD_FILE_NAME_LIST
    if node.kind == CursorKind.FUNCTION_DECL:
        if is_has_extern_in_node(node):
            HEAD_FILE_NAME_LIST.append(node.spelling)
    if node.kind == CursorKind.VAR_DECL and depth == 1:
        if is_has_extern_in_node(node):
            HEAD_FILE_NAME_LIST.append(node.spelling)
    for n in node.get_children():
        traverse_c(n, depth + 1)


def check_cpp_namespace(depth):
    if not depth.find('std') == -1:
        return False
    if not depth.find('__gnu_cxx') == -1:
        return False
    if not depth.find('__cxxabiv1') == -1:
        return False
    if not depth.find('__pthread_cleanup_class') == -1:
        return False
    return True


def traverse_cpp(node, depth):
    '''
    Recursively obtain the symbol list from the c++ language header file and store it in HEAD_FILE_NAME_LIST
    '''
    global HEAD_FILE_NAME_LIST
    if node.kind == CursorKind.NAMESPACE or node.kind == CursorKind.CLASS_DECL:
        if depth == '':
            depth = node.spelling
        else:
            depth = depth + '::' + node.spelling
    if node.kind == CursorKind.CXX_METHOD and check_cpp_namespace(
            depth) and not depth == '' and not node.is_pure_virtual_method():
        HEAD_FILE_NAME_LIST.append(depth + '::' + node.spelling + '@' +
                                   str(get_permission_num(node.displayname)))
    if node.kind == CursorKind.FUNCTION_TEMPLATE and check_cpp_namespace(
            depth) and not depth == '':
        HEAD_FILE_NAME_LIST.append(depth + '::' + node.spelling + '@' +
                                   str(get_permission_num(node.displayname)))
    if node.kind == CursorKind.DESTRUCTOR and check_cpp_namespace(depth):
        HEAD_FILE_NAME_LIST.append(depth + '::' + node.spelling + '@0')
    if node.kind == CursorKind.VAR_DECL and check_cpp_namespace(
            depth) and not depth == '':
        HEAD_FILE_NAME_LIST.append(depth + '::' + node.spelling)
    if node.kind == CursorKind.CONSTRUCTOR and check_cpp_namespace(depth):
        HEAD_FILE_NAME_LIST.append(depth + '::' + node.spelling + '@' +
                                   str(get_permission_num(node.displayname)))
    for n in node.get_children():
        traverse_cpp(n, depth)


def is_has_extern_in_node(node):
    for i in node.get_tokens():
        if i.spelling == 'extern' or i.spelling == '__inline':
            return False
    return True


def traverse(node):
    '''
    Determine the type of the file parameter
    '''
    global CURRENT_FILE_TYPE
    if node.kind == CursorKind.CLASS_DECL:
        CURRENT_FILE_TYPE = 1
    for n in node.get_children():
        traverse(n)


def get_compare_result():
    '''
    Compare the symbol lists of header and library files and generate a file that stores the results of the comparison
    '''
    only_lib_have = list(
        set(LIB_FILE_NAME_LIST).difference(set(HEAD_FILE_NAME_LIST)))
    only_head_have = list(
        set(HEAD_FILE_NAME_LIST).difference(set(LIB_FILE_NAME_LIST)))
    not_compare_list = []
    if not INPUT_BLACK_PATH == '':
        not_compare_list = not_compare_list + get_info_from_file(
            INPUT_BLACK_PATH)
    only_lib_have = list(set(only_lib_have).difference(set(not_compare_list)))
    only_head_have = list(
        set(only_head_have).difference(set(not_compare_list)))
    only_lib_have.sort()
    only_head_have.sort()
    result = {}
    result['head_file_path'] = INPUT_HEAD_PATH
    result['lib_file_path'] = INPUT_LIB_PATH
    result['only_in_head_file'] = only_head_have
    result['only_in_lib_file'] = only_lib_have
    result['head_file_symble_list_num:lib_file_symble_list_nmu'] = str(
        len(HEAD_FILE_NAME_LIST)) + ':' + str(len(LIB_FILE_NAME_LIST))
    seconds = time.time()
    time_str = time.strftime('%Y-%m-%d-%H:%M:%S', time.localtime(seconds))
    out_difference_path = OUTPUT_RESULT_PATH + 'compare_result_' + time_str + '.json'
    out_lib_path = OUTPUT_RESULT_PATH + 'lib_file_list_' + time_str + '.txt'
    out_head_path = OUTPUT_RESULT_PATH + 'head_file_list_' + time_str + '.txt'
    with open(out_difference_path, 'w') as file_obj:
        json.dump(result, file_obj, indent=4, separators=(',', ':'))
        file_obj.close()
    f = open(out_lib_path, 'w')
    for line in LIB_FILE_NAME_LIST:
        f.write(line + '\n')
    f.close()
    f = open(out_head_path, 'w')
    for line in HEAD_FILE_NAME_LIST:
        f.write(line + '\n')
    f.close()


INPUT_LIB_PATH = '' #The path to the entered library file
INPUT_HEAD_PATH = '' #The path to the entered header file
INPUT_BLACK_PATH = '' #The path to the entered blacklist file
INPUT_MACROS_PATH = '' #The path to the entered macro definition file
OUTPUT_RESULT_PATH = '' #The output path of the result file
HEAD_FILE_NAME_LIST = [] #header file symbol list
LIB_FILE_NAME_LIST = [] #library file symbol list
CURRENT_FILE_TYPE = 0 #current file type
get_args()
get_head_strs()
get_lib_strs()
get_compare_result()
