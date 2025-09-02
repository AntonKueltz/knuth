from setuptools import setup, Extension
from os import environ

extra_compile_args = ["-std=c99"]
extra_link_args = []

if environ.get("COVERAGE_BUILD"):
    extra_compile_args.extend(["--coverage", "-g", "-O0"])
    extra_link_args.extend(["--coverage"])
else:
    extra_compile_args.extend(["-O2"])

ext_modules = [
    Extension(
        "knuth.binary_tree",
        ["knuth/binaryTree.c"],
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
    ),
    Extension(
        "knuth.linked_list",
        ["knuth/linkedList.c"],
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
    ),
]

setup(ext_modules=ext_modules)
