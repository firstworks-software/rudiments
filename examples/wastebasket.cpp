#include <rudiments/wastebasket.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	wastebasket	wb;

	// attach dynamically allocated strings...
	char	*str1=charstring::duplicate("hello");
	char	*str2=charstring::duplicate("goodbye");
	wb.attach(str1);
	wb.attach(str2);

	// the strings are still usable...
	stdoutput.printf("str1: %s\n",str1);
	stdoutput.printf("str2: %s\n",str2);
	stdoutput.write('\n');


	// attach dynamically allocated arrays of primitives...
	int64_t	*nums=new int64_t[3];
	nums[0]=100;
	nums[1]=200;
	nums[2]=300;
	wb.attach(nums);

	stdoutput.printf("nums: %lld, %lld, %lld\n",
			nums[0],nums[1],nums[2]);
	stdoutput.write('\n');


	// attach a dynamically allocated object...
	stringbuffer	*sb=new stringbuffer();
	sb->append("hello from stringbuffer");
	wb.attach(sb);

	stdoutput.printf("sb: %s\n",sb->getString());
	stdoutput.write('\n');


	// empty the wastebasket, deleting all attached values...
	stdoutput.printf("emptying the wastebasket...\n");
	wb.empty();
	stdoutput.printf("done\n");

	// str1, str2, nums, and sb are no longer valid
}
