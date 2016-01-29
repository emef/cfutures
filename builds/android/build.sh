#!/usr/bin/env bash
################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Please refer to the README for information about making permanent changes.  #
################################################################################

# Use directory of current script as the build directory and working directory
cd "$( dirname "${BASH_SOURCE[0]}" )"
ANDROID_BUILD_DIR="$(pwd)"

# Get access to android_build functions and variables
source ${ANDROID_BUILD_DIR}/android_build_helper.sh

# Choose a C++ standard library implementation from the ndk
ANDROID_BUILD_CXXSTL="gnustl_shared_48"

# Set up android build environment and set ANDROID_BUILD_OPTS array
android_build_env
android_build_opts

# Use a temporary build directory
cache="/tmp/android_build/${TOOLCHAIN_NAME}"
rm -rf "${cache}"
mkdir -p "${cache}"

# Check for environment variable to clear the prefix and do a clean build
if [[ $ANDROID_BUILD_CLEAN ]]; then
    echo "Doing a clean build (removing previous build and depedencies)..."
    rm -rf "${ANDROID_BUILD_PREFIX}"/*
fi

##
# Build cfutures from local source

(android_build_verify_so "libcfutures.so" &> /dev/null) || {
    rm -rf "${cache}/cfutures"
    (cp -r ../.. "${cache}/cfutures" && cd "${cache}/cfutures" \
        && make clean && rm -f configure config.status)

    export LIBTOOL_EXTRA_LDFLAGS='-avoid-version'

    (cd "${cache}/cfutures" && ./autogen.sh \
        && ./configure "${ANDROID_BUILD_OPTS[@]}" --without-documentation \
        && make -j 4 \
        && make install) || exit 1
}

##
# Verify shared libraries in prefix

android_build_verify_so "libcfutures.so"
echo "Android build successful"
################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Please refer to the README for information about making permanent changes.  #
################################################################################