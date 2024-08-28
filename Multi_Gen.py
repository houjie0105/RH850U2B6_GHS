# -*- coding: utf-8 -*-
"""
chaoyang.ren.aj@renesas.com
"""

import os

PRJ_NAME = "default"
PROGRAM = "ghs_u2b6_mcal"
CORE_NUM = 3
TARGET_ARCH = "rh850g4mh"
OUTPUT_DIR = "output"

IGNORE_DIR = [".git",
              ".vscode",
              "tools",
              "output"]

IGNORE_FILE = ["example.c"]

PRJ_FILE_SUFFIX = ".gpj"

SRC_FILE_SUFFIX = ".c"
ASM_FILE_SUFFIX = ".850"
LIB_FILE_SUFFIX = ".a"
LD_FILE_SUFFIX = ".ld"
INC_FILE_SUFFIX = ".h"
GHSMC_FILE_SUFFIX = ".ghsmc"
GPC_FILE_SUFFIX = ".gpc"
ELF_FILE_SUFFIX = ".elf"

PROJ_FILE_TYPE = [SRC_FILE_SUFFIX, ASM_FILE_SUFFIX, \
    LIB_FILE_SUFFIX, LD_FILE_SUFFIX, INC_FILE_SUFFIX]

DEBUG = "debug"
RELEASE = "release"

TOP_GPJ = \
'''#!gbuild
defineConfig(Debug   dbg debug.gpc)
defineConfig(Release rel release.gpc)
primaryTarget=v800_standalone.tgt
[Project]
%s.gpj\t[Program]
'''
PROGRAM_GPJ = \
'''
#!gbuild
[Program]
'''
GHSMC_S = \
'''version = 1
title = "%s"

core {
\t0 { download = {"./%s/%s%s"} }
'''
GHSMC_COM = \
'''\t%d { symbol = {"./%s/%s%s"} }
'''
GHSMC_E = \
'''}
'''
DEBUG_GPC = \
'''#!gbuild
[Build Configuration]
\t-bsp generic
\t-cpu=%s
\t-no_misalign_pack
\t-overlap
\t-G
\t-e _RESET
\t-prepare_dispose
\t-inline_prologue
\t-sda=all
\t-large_sda
\t-dual_debug
\t--diag_suppress 1,47
\t-D__DEBUG
'''
RELEASE_GPC = \
'''#!gbuild
[Build Configuration]
\t-bsp generic
\t-cpu=%s
\t-no_misalign_pack
\t-overlap
\t-e _RESET
\t-prepare_dispose
\t-inline_prologue
\t-sda=all
\t-large_sda
\t--diag_suppress 1,47
'''

DSS = '/'
DSSTW = '\\'

IH = "\t-I.\\"

NL = "\n"

PFH = \
'''#!gbuild
'''
SPCO = \
'''[Subproject]
'''
PCO = \
'''[Program]
\t{optgroup=GhsCommonOptions} -o ./%s/%s%s
\t-object_dir=./%s/objs
\t-ghsmc_file=./%s%s
'''

FXU = '_fxu'
USE_FXU = "\t-rh850_fxu"

TOP_DIR = 0

def add_src_file(cur_dir, dir_depth):

    if dir_depth == 0:
        project_file = open(cur_dir + DSS + PROGRAM \
            + PRJ_FILE_SUFFIX, 'w')
        project_file.write(PFH)
        project_file.write((PCO) % (OUTPUT_DIR, PROGRAM, ELF_FILE_SUFFIX, OUTPUT_DIR, PROGRAM, GHSMC_FILE_SUFFIX))
        project_file.write(IH + NL)
        for f_root, f_dirs, f_files in os.walk(cur_dir):
            for f_dir in f_dirs:
                inc_path = os.path.relpath(os.path.join(f_root, f_dir))
                dir_lists = inc_path.split(DSSTW)
                if dir_lists[0] in IGNORE_DIR:
                    pass
                else:
                    project_file.write(IH \
                        + os.path.relpath(os.path.join(f_root, f_dir)) \
                        + NL)

    else:
        project_file_name = os.path.basename(cur_dir)
        project_file = open(cur_dir + DSS + project_file_name \
            + PRJ_FILE_SUFFIX, 'w')
        project_file.write(PFH)
        project_file.write(SPCO)
    
    for file_list in os.listdir(cur_dir):
        full_path = os.path.join(cur_dir, file_list)
        dir_name = os.path.basename(full_path)

        if os.path.isdir(full_path):
            if dir_name in IGNORE_DIR:
                pass
            else:
                project_file.write(dir_name + DSSTW + \
                    dir_name + PRJ_FILE_SUFFIX + NL)
                dir_depth += 1
                add_src_file(full_path, dir_depth)

        else:
            cur_file_suffix = os.path.splitext(full_path)[1]
            if cur_file_suffix in PROJ_FILE_TYPE:
                if dir_name in IGNORE_FILE:
                    pass
                else:
                    project_file.write(dir_name + NL)
                    if FXU in dir_name:
                        project_file.write(USE_FXU + NL)
    project_file.close()


def project_generator():
    project_file = open(os.getcwd() + DSS + PRJ_NAME + PRJ_FILE_SUFFIX, 'w')
    project_file.write((TOP_GPJ) % (PROGRAM))
    project_file.close()
    add_src_file(os.getcwd(), TOP_DIR)
    if CORE_NUM > 1:
        project_file = open(os.getcwd() + DSS + PROGRAM + GHSMC_FILE_SUFFIX, 'w')    
        project_file.write((GHSMC_S) % (PROGRAM, OUTPUT_DIR, PROGRAM, ELF_FILE_SUFFIX))
        for i in range(1, CORE_NUM):
            project_file.write((GHSMC_COM) % (i, OUTPUT_DIR, PROGRAM, ELF_FILE_SUFFIX))
        project_file.write(GHSMC_E)
        project_file.close()
    project_file = open(os.getcwd() + DSS + RELEASE + GPC_FILE_SUFFIX, 'w')
    project_file.write((RELEASE_GPC) % (TARGET_ARCH))
    project_file.close()
    project_file = open(os.getcwd() + DSS + DEBUG + GPC_FILE_SUFFIX, 'w')
    project_file.write((DEBUG_GPC) % (TARGET_ARCH))
    project_file.close()

# if __name__ == '__main__':
project_generator()
