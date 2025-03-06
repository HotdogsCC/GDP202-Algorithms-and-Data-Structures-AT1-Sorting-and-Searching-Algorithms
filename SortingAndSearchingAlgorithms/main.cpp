/*
 * Sorting and Searching v1.01
 *
 * Algorithms and Data Structures
 * Copyright (C) 2024 AIE Institute
 *
 * This code is for academic purposes only and is not to be redistibuted
 *
 */

#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <chrono>
#include <bitset>
#include <assert.h>
#include <cmath>
#include <unordered_set>

#include <random>
using DataContainer = std::vector<unsigned int>;


/*******************************************************************************/

namespace Sort
{
    // Note. The data is passed in by reference to avoid an expensive copy.
    // This lends itself nicely to an in-place sort implementation.
    // If you make a temp copy to do your sorting, remember to overwrite the original data before returning

    void InsertionSort(DataContainer& data) {
        // Write your implementation of Insertion sort here
    }

    void MergeSort(DataContainer& data) {
        // Write your implementation of Merge sort here
    }

    /************************************************************/
    /* Add your chosen sort method implementation and name here */
    /************************************************************/

    const char* YourChosenSortName = /**/"Your Sort name goes here"/**/;

    void YourChosenSort(DataContainer& data) {
        // Write your implementation of your chosen sort here
    }

    constexpr int maximum_elements = 100000;
    constexpr int step_multiplier = 10;
}

namespace Search
{
    // Write your implementation here
    // Return -1 if not found, otherwise return the index where the element is stored.
    int LinearSearch(const DataContainer& data, unsigned int value) {

        //checks each item in data until the item is found
        for (int i = 0; i < data.size(); i++)
        {
            if (data[i] == value) //checks if current item is the one we're looking for
            {
                return i; //returns current index
            }
        }

        return -1; //number is not in the array
    }

    // Write your implementation here
    // Return -1 if not found, otherwise return the index where the element is stored.
    int BinarySearch(const DataContainer& data, unsigned int value) {

        //searches for a number from a list sorted from smallest to largest

        int low = 0;
        int high = data.size();

        while (low <= high) //if the number does not exist in the vector, low will become bigger than high
        {
            int middle = (low + high) / 2; //check index in the middle of low and high bounds
            if (value == data[middle])
            {
                return middle;
            }
            else if (value < data[middle]) //occurs if the value is smaller than what was tested
            {
                high = middle - 1;
            }
            else //occurs if the value is bigger than what was tested
            {
                low = middle + 1;
            }
        }

        return -1; //number is not in the array
    }

    constexpr int maximum_elements = 100000;
    constexpr int step_multiplier = 2;
}

/*******************************************************************************
 *
 *                    Do not change any code below here
 *
 *******************************************************************************/

namespace Sort {
    // Check the data is sorted in order
    bool IsSorted(const DataContainer& data) {
        for (int i = 1; i < data.size(); ++i) {
            if (data[i] < data[i - 1])
                return false;
        }
        return true;
    }
}

namespace Random {
    constexpr int seed = 49152; //We want deterministic behaviour
    static std::default_random_engine dre{ seed };
    static std::uniform_int_distribution<unsigned int> uid{ 0,1 << 24 };

    /// <summary>
    /// This function generates a number of random integers and returns them in a DataContainer object
    /// </summary>
    /// <param name="count">the number of items to generate</param>
    /// <returns>The list of random values</returns>
    DataContainer MakeRandom(int count) {
        DataContainer output;
        output.reserve(count);
        for (int i = 0; i < count; ++i) {
            output.push_back(uid(dre));
        }
        return output;
    }

    DataContainer PickValuesInContainer(DataContainer data, int count) {
        ///std::shuffle(data.begin(), data.end(), dre);

        DataContainer values;
        if (data.size() <= count)
        {
            std::cerr << "Too many values" << std::endl;
            abort();
        }

        const double step_size = static_cast<double>((data.size() - 1) / (count - 1));

        for (int i = 0; i < count; i++) {
            values.push_back(data[static_cast<unsigned int>(i * step_size)]);
        }
        return values;
    }

    DataContainer PickValuesNotInContainer(const DataContainer& data, int count) {
        std::unordered_set<unsigned int> values{ data.begin(),data.end() };
        DataContainer output_values;
        while (values.size() < count) {
            auto v = uid(dre);
            if (values.find(v) == values.end())
                output_values.push_back(v);
        }
        return output_values;
    }
}

namespace Timer {
    std::chrono::time_point<std::chrono::steady_clock> then;
    //Start time point
    void start_timer() {
        then = std::chrono::steady_clock::now();
    }

    // get time in milliseconds 
    double end_timer() {
        const auto end = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(end - then).count();
    }
}

namespace Data {

    // Test structure
    struct TestData
    {
        const std::string test_name;
        const DataContainer data;
    };

    void OutputTestData(const TestData& test) {
        std::string filename = test.test_name + std::string{ "_" } + std::to_string(test.data.size()) + ".txt";

        auto file = std::ofstream(filename);
        for (auto v : test.data)
            file << v << " , ";
        file.close();
    }

    //Random numbers
    std::vector<TestData> GenerateTestData() {

        std::vector<TestData> testdata;
        using Random::dre;
        using Random::uid;

        std::cout << "Generating Test data..." << std::endl;

        for (int i = 10; i <= Sort::maximum_elements; i *= Sort::step_multiplier) {

            std::cout << i << " elements data" << std::endl;
            const int data_size = i;

            // Fully random
            const auto random_data = Random::MakeRandom(data_size);
            testdata.emplace_back(TestData{ "random", random_data });
            OutputTestData(testdata.back());

            // Uniform data
            auto uniform_data = random_data;
            for (auto& v : uniform_data)
                v = (static_cast<unsigned int>(-1) >> 1);
            testdata.emplace_back(TestData{ "uniform", uniform_data });
            OutputTestData(testdata.back());

            // Sorted
            auto sorted_data = random_data;
            std::sort(sorted_data.begin(), sorted_data.end());
            testdata.emplace_back(TestData{ "ordered", sorted_data });
            OutputTestData(testdata.back());

            // Reverse sorted
            auto data = sorted_data;
            std::reverse(data.begin(), data.end());
            testdata.emplace_back(TestData{ "reverse_ordered", data });
            OutputTestData(testdata.back());

            // first 10th shuffled
            data = sorted_data;
            std::shuffle(data.begin(), data.begin() + i / 10, dre);
            testdata.emplace_back(TestData{ "shuffle_first_10th", data });
            OutputTestData(testdata.back());

            // last 10th shuffled
            data = sorted_data;
            std::shuffle(data.begin() + i * 9 / 10, data.end(), dre);
            testdata.emplace_back(TestData{ "shuffle_last_10th", data });
            OutputTestData(testdata.back());

            // Almost sorted
            data = sorted_data;
            for (int j = 0; j < i / 10; j++) {
                std::swap(data[Random::uid(dre) % data.size()], data[uid(dre) % data.size()]);
            }
            testdata.emplace_back(TestData{ "almost_sorted", data });
            OutputTestData(testdata.back());

            // k-sorted
            data = sorted_data;
            const int k = std::max(data_size / 100, 1);
            for (int j = 0; j < data_size; j++) {

                const auto min_range = std::max(0, j - k);
                const auto max_range = std::min(data_size - 1, j + k);

                int swap_index = (Random::uid(dre) % (max_range - min_range)) + min_range;
                std::swap(data[j], data[swap_index]);
            }
            testdata.emplace_back(TestData{ "k_sorted", data });
            OutputTestData(testdata.back());

            //Alternate high-low
            data = sorted_data;
            int l = 0; int r = data_size - 1;
            for (int j = 0; r > l; j++) {
                if (j % 2 == 0)
                    data[j] = sorted_data[l++];
                else
                    data[j] = sorted_data[r--];
            }
            testdata.emplace_back(TestData{ "zig-zag", data });
            OutputTestData(testdata.back());
        }
        return testdata;
    }
}

namespace Tests
{
    struct SortingTests
    {
        std::string test_name;
        void(*Sort)(DataContainer&);
    };

    std::vector<SortingTests> GenerateTests() {
        std::vector<SortingTests> tests = {
        {"InsertionSort"            , Sort::InsertionSort  },
        {"MergeSort"                , Sort::MergeSort      },
        {Sort::YourChosenSortName   , Sort::YourChosenSort }
        };
        return tests;
    }
}

int main(int argv, char** argc) {



    //Generate the tests
    const auto all_tests = Data::GenerateTestData();
    std::vector<unsigned int> td;
    Data::TestData mytes{ "ha", td };

    //Set up the three sorts
    const auto sort_tests = Tests::GenerateTests();

    //Set up output 
    std::ostringstream sort_output_string;

    std::cout << "\n\nRunning the tests" << std::endl;

    sort_output_string << "Test,Elements";
    for (int i = 0; i < sort_tests.size(); ++i) {
        sort_output_string << ",";
        sort_output_string << sort_tests[i].test_name;
    }

    sort_output_string << std::endl;

    //Run the tests
    for (const auto& test : all_tests) {
        //output_string << "\n\nTest Name : " << test.test_name << std::endl;
        sort_output_string << test.test_name << "," << test.data.size() << ",";
        //Perform all three sorts
        for (int i = 0; i < sort_tests.size(); ++i) { // const auto & t : sort_tests) {
            const auto& t = sort_tests[i];

            auto data = test.data;

            //Start the timer
            Timer::start_timer();

            //Do the sorting
            t.Sort(data);

            //End the Timer
            auto elapsed = Timer::end_timer();

            //Confirm it's sorted
            if (!Sort::IsSorted(data)) {
                std::cerr << "Data is not sorted, check your " << t.test_name << " implementation" << std::endl;
                abort();
            }

            //Output the time
            sort_output_string << elapsed;
            if (i < sort_tests.size() - 1)
                sort_output_string << ",";
        }
        sort_output_string << std::endl;
    }

    //Set up output 
    std::ostringstream search_output_string;
    search_output_string << "Elements,Linear,Binary" << std::endl;


    // Run the searching functions
    for (int i = 10; i <= Search::maximum_elements; i *= Search::step_multiplier) {
        search_output_string << i;

        auto values = Random::MakeRandom(i);
        std::sort(values.begin(), values.end());

        //For each search function, try to find 4 elements in the set, and 4 elements out of the set
        const auto pick_values = Random::PickValuesInContainer(values, 4);
        const auto not_pick_values = Random::PickValuesNotInContainer(values, 4);

        DataContainer search_values;
        search_values.reserve(pick_values.size() + not_pick_values.size());
        search_values.insert(search_values.end(), pick_values.begin(), pick_values.end());
        search_values.insert(search_values.end(), not_pick_values.begin(), not_pick_values.end());

        //output_string << "Linear Search " << i << " elements :";

        //Start the timer
        Timer::start_timer();
        for (auto p : search_values)
            Search::LinearSearch(values, p);
        auto elapsed = Timer::end_timer();

        search_output_string << "," << elapsed;

        Timer::start_timer();
        for (auto p : search_values)
            Search::BinarySearch(values, p);
        elapsed = Timer::end_timer();

        search_output_string << "," << elapsed;

        //Test the search works
        for (auto p : pick_values)
        {
            auto index = Search::LinearSearch(values, p);
            if (index == -1) {
                std::cerr << "Linear search failed to find " << p << " with " << i << " number of elements." << std::endl;
                //abort();
            }
            else if (values[index] != p)
            {
                std::cerr << "Linear search failed with value " << p << " with " << i << " number of elements." << std::endl;
                //abort();
            }

            index = Search::BinarySearch(values, p);
            if (index == -1) {
                std::cerr << "Binary search failed to find " << p << " with " << i << " number of elements." << std::endl;
                //abort();
            }
            else if (values[index] != p)
            {
                std::cerr << "Binary search failed with value " << p << " with " << i << " number of elements." << std::endl;
                //abort();
            }
        }

        //Test the search works
        for (auto p : not_pick_values)
        {
            auto index = Search::LinearSearch(values, p);
            if (values[index] != -1)
            {
                std::cerr << "Linear search should not have found " << p << " with " << i << " number of elements." << std::endl;
                abort();
            }

            index = Search::BinarySearch(values, p);
            if (values[index] != p)
            {
                std::cerr << "Binary search should not have found " << p << " with " << i << " number of elements." << std::endl;
                abort();
            }
        }

        search_output_string << std::endl;
    }

    std::cout << "\n\nOutput" << std::endl << std::endl;

    //Sort
    std::cout << "\n\nSort results\n\n";
    std::cout << sort_output_string.str();

    std::ofstream output{ "sort_results.csv" };
    output << sort_output_string.str();
    output.close();

    //Search
    std::cout << "\n\nSearch results\n\n";
    std::cout << search_output_string.str();

    output.open("search_results.csv");
    output << search_output_string.str();
    output.close();

    return EXIT_SUCCESS;
}