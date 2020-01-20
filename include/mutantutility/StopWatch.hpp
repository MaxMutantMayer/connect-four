/*
 * Mutant Utility - StopWatch
 * 
 * Simple 'header-only' stop watch implementation using std::chrono.
 *
 * Author:	Maximilian Mayer
 * License:	MIT (see end of file)
 */

#ifndef MUTANTUTILITY_STOPWATCH_H
#define MUTANTUTILITY_STOPWATCH_H

#include <chrono>

namespace mutant_utility
{

/*
 * Measure execution times in seconds/milliseconds.
 * Time measurement starts on object creation and can be reset with 'Start'.
 */
class StopWatch
{
public:
	inline StopWatch(void)
	{
		Start();
	}

	inline void Start(void)
	{
		start = std::chrono::high_resolution_clock::now();
	}

	inline double ToSeconds(void) const
	{
		std::chrono::high_resolution_clock::time_point now =
			std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::ratio<1>> elapsed_seconds =
			now - start;
		return elapsed_seconds.count();
	}

	inline double ToMilliseconds(void) const
	{
		std::chrono::high_resolution_clock::time_point now =
			std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::ratio<1, 1000>> elapsed_milliseconds =
			now - start;
		return elapsed_milliseconds.count();
	}

private:
	std::chrono::high_resolution_clock::time_point start;
};

} // namespace mutant_utility

#endif // !MUTANTUTILITY_STOPWATCH_H


// Copyright (c) 2020 Maximilian Mayer
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
