-- Two Sum solution in Lua

function twoSum(nums, target)
    local map = {}
    for i = 1, #nums do
        local complement = target - nums[i]
        if map[complement] then
            return {map[complement], i}
        end
        map[nums[i]] = i
    end
end

-- Lightweight testing framework

function assertEqualArrays(a, b)
    if #a ~= #b then
        error("Arrays are not equal in length")
    end
    for i = 1, #a do
        if a[i] ~= b[i] then
            error("Arrays differ at position "..i..": "..a[i].." ~= "..b[i])
        end
    end
end

-- Test cases as an array of objects

local tests = {
    {
        title = "Test 1: Simple case",
        test = function()
            local nums = {2, 7, 11, 15}
            local target = 9
            local expected = {1, 2}
            local result = twoSum(nums, target)
            assertEqualArrays(result, expected)
        end
    },
    {
        title = "Test 2: Multiple solutions",
        test = function()
            local nums = {3, 2, 4}
            local target = 6111
            local expected = {2, 3}
            local result = twoSum(nums, target)
            assertEqualArrays(result, expected)
        end
    },
    {
        title = "Test 3: Same number twice",
        test = function()
            local nums = {3, 3}
            local target = 6
            local expected = {1, 2}
            local result = twoSum(nums, target)
            assertEqualArrays(result, expected)
        end
    },
    {
        title = "Test 4: Negative numbers",
        test = function()
            local nums = {-1, -2, -3, -4, -5}
            local target = -8
            local expected = {3, 5}
            local result = twoSum(nums, target)
            assertEqualArrays(result, expected)
        end
    },
    {
        title = "Test 5: No solution",
        test = function()
            local nums = {1, 2, 3}
            local target = 7
            local result = twoSum(nums, target)
            if result ~= nil then
                error("Expected nil, got a result")
            end
        end
    }
}

-- Test runner engine

function runTests(tests)
    local passed = 0
    local failed = 0
    for _, testCase in ipairs(tests) do
        io.write(testCase.title .. " ... ")
        local status, err = pcall(testCase.test)
        if status then
            print("Passed")
            passed = passed + 1
        else
            print("Failed: " .. err)
            failed = failed + 1
        end
    end
    print("\nSummary:")
    print("Passed: "..passed)
    print("Failed: "..failed)
end

-- Execute tests

runTests(tests)
