from setuptools import setup, Extension
import os

extra_compile_args = ["-std=c99", "-O2"]
extra_link_args = []

if os.environ.get("COVERAGE"):
    extra_compile_args.extend(["--coverage", "-g"])
    extra_link_args.extend(["--coverage"])

ext_modules = [
    Extension(
        "knuth.binary_tree",
        ["knuth/binaryTree.c"],
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
    )
]

setup(ext_modules=ext_modules)
