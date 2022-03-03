#include "cgp/containers/buffer/buffer.hpp"

namespace cgp_test 
{

	void text_buffer()
	{
		{
			cgp::buffer<int> a = { 1,2,3 };
			assert_cgp_no_msg(is_equal(a, { 1,2,3 }));
			assert_cgp_no_msg(!is_equal(a, { 1,2,3,4 }));
			assert_cgp_no_msg(!is_equal(a, { 1,2 }));
		}

		{
			cgp::buffer<float> a = { 1.1f, 2.2f, 3.1f };
			assert_cgp_no_msg(is_equal(a, { 1.1f,2.2f,3.1f }));
			assert_cgp_no_msg(!is_equal(a, { 1.1f,2.2f,3.1f,4.5f }));
			assert_cgp_no_msg(!is_equal(a, { 1.1f,2.2f }));
			assert_cgp_no_msg(!is_equal(a, { 1.1f,2.21f,3.1f }));
		}

		{
			cgp::buffer<int> a = { 1,2,3 };
			a[0] = -1;
			a[1] = 5;
			a[2] = a[1] + 1;
			assert_cgp_no_msg(is_equal(a, { -1,5,6 }));
			a = { 7,8,-2,3 };
			assert_cgp_no_msg(is_equal(a, { 7,8,-2,3 }));
			a.push_back(9);
			assert_cgp_no_msg(is_equal(a, { 7,8,-2,3,9 }));
			a.resize_clear(2);
			assert_cgp_no_msg(is_equal(a, { 0,0 }));
		}

		{
			cgp::buffer<int> const a = { 1,2,3 };
			assert_cgp_no_msg(a[0] == 1); assert_cgp_no_msg(a(0) == 1); assert_cgp_no_msg(a.at(0) == 1);
			assert_cgp_no_msg(a[1] == 2); assert_cgp_no_msg(a(1) == 2); assert_cgp_no_msg(a.at(1) == 2);
			assert_cgp_no_msg(a[2] == 3); assert_cgp_no_msg(a(2) == 3); assert_cgp_no_msg(a.at(2) == 3);
		}

		{
			cgp::buffer<int> a = { 1,2,3 };
			a[0] = 5; a[1] = 6; a[2] = 7;
			assert_cgp_no_msg(is_equal(a, { 5,6,7 }));
			a(0) = 2; a(1) = 1; a(2) = -7;
			assert_cgp_no_msg(is_equal(a, { 2,1,-7 }));
			a.at(0) = -1; a.at(1) = 9; a.at(2) = -7;
			assert_cgp_no_msg(is_equal(a, { -1,9,-7 }));
		}


		{
			cgp::buffer<cgp::buffer<int>> a = { {1,2},{2,3,4}, {-1} };
			assert_cgp_no_msg(a[0].size() == 2); assert_cgp_no_msg(is_equal(a[0], { 1,2 }));
			assert_cgp_no_msg(a[1].size() == 3); assert_cgp_no_msg(is_equal(a[1], { 2,3,4 }));
			assert_cgp_no_msg(a[2].size() == 1); assert_cgp_no_msg(is_equal(a[2], { -1 }));
			a.push_back(cgp::buffer<int>{ 7,8,9 });
			assert_cgp_no_msg(a[3].size() == 3); assert_cgp_no_msg(is_equal(a[3], { 7,8,9 }));
		}

		{
			cgp::buffer<int> a; assert_cgp_no_msg(cgp::is_equal(type_str(a),"buffer<int>"));
			cgp::buffer<float> b; assert_cgp_no_msg(cgp::is_equal(type_str(b), "buffer<float>"));
			cgp::buffer<cgp::buffer<int>> c; assert_cgp_no_msg(cgp::is_equal(type_str(c), "buffer<buffer<int>>"));
		}

		{
			cgp::buffer<int> a = { 4,5,6 };
			assert_cgp_no_msg(cgp::is_equal(str(a), "4 5 6"));
			assert_cgp_no_msg(cgp::is_equal(str(a,","), "4,5,6"));
			assert_cgp_no_msg(cgp::is_equal(str(a, ",","[","]"), "[4,5,6]"));
		}

		{
			cgp::buffer<int> a = { 4,5,6 };
			int counter = 0;
			for (int v : a) {
				if (counter == 0) assert_cgp_no_msg(v == 4);
				if (counter == 1) assert_cgp_no_msg(v == 5);
				if (counter == 2) assert_cgp_no_msg(v == 6);
				counter++;
			}
			assert_cgp_no_msg(counter==3);
		}

		{
			cgp::buffer<int> a = { 7,5,-1 };
			assert_cgp_no_msg(cgp::size_in_memory(a)==3*sizeof(int));

			cgp::buffer<cgp::buffer<int>> b = { {1,2},{2,3,4}, {-1} };
			assert_cgp_no_msg(cgp::size_in_memory(b) == 6 * sizeof(int));
		}

		// test linspace
		{
			cgp::buffer<float> a = cgp::buffer<float>::linespace(4.5f, 8.2f, 6);
			assert_cgp_no_msg(a.size() == 6);
			assert_cgp_no_msg(cgp::is_equal(a[0], 4.5f));
			assert_cgp_no_msg(cgp::is_equal(a[5], 8.2f));
		}

	}
}