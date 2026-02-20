#include <rudiments/scalar.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// store an integer value
	scalar<int64_t>	i;
	i.setValue(4);
	stdoutput.printf("integer value: %lld\n",i.getValue());

	// update the value
	i.setValue(8);
	stdoutput.printf("updated value: %lld\n\n",i.getValue());

	// clear the value
	i.clear();
	stdoutput.printf("after clearing: %lld\n\n",i.getValue());

	// store a string value
	scalar<const char *>	s;
	s.setValue("dog");
	stdoutput.printf("string value: %s\n",s.getValue());

	s.setValue("eagle");
	stdoutput.printf("updated value: %s\n\n",s.getValue());

	// copy a scalar
	scalar<int64_t>	a;
	a.setValue(10);
	scalar<int64_t>	b(a);
	stdoutput.printf("original: %lld\n",a.getValue());
	stdoutput.printf("copy: %lld\n",b.getValue());
}
