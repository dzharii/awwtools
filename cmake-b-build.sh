#!/bin/env bash
cmake --build cmake-build && ./cmake-build/unit_tests && ./cmake-build/unit_tests --list-test-cases