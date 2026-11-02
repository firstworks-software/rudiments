// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/table.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/signalclasses.h>
#include "test.cpp"

char	cn[2];
const char	*colname(uint64_t col) {
	cn[0]='A'+(col%26);
	cn[1]='\0';
	return cn;
}

// block-based source, loads a block of rows when a row
// past the current block is read with getValue()
const char	*blocknames[]={"A","B","C"};
class blocktable : public tablecollection<int32_t> {
	public:
		blocktable(uint64_t rows, uint64_t cols,
				uint64_t blocksize, bool nevertrue) :
				tablecollection<int32_t>() {
			this->rows=rows;
			this->cols=cols;
			this->blocksize=blocksize;
			this->nevertrue=nevertrue;
			loaded=0;
			allrows=false;
			dummy=0;
			loadBlock();
		}
		void	setColumnName(uint64_t col, const char *name) {
		}
		const char	*getColumnName(uint64_t col) {
			return (col<cols)?blocknames[col]:NULL;
		}
		uint64_t	getColumnCount() {
			return cols;
		}
		void	setValue(uint64_t row, uint64_t col, int32_t value) {
		}
		int32_t	getValue(uint64_t row, uint64_t col) {
			if (row>=loaded && !allrows) {
				loadBlock();
			}
			return (row<loaded && col<cols)?value(row,col):0;
		}
		int32_t	&getReference(uint64_t row, uint64_t col) {
			return dummy;
		}
		int32_t	getValue(uint64_t row, const char *colname) {
			return 0;
		}
		uint64_t	getRowCount() {
			return loaded;
		}
		bool	getAllRowsAvailable() {
			return (nevertrue)?false:allrows;
		}
		bool	isBlockBased() {
			return true;
		}
		bool	clear() {
			return true;
		}
		static int32_t	value(uint64_t row, uint64_t col) {
			return (int32_t)(row*10+col+1);
		}
	private:
		void	loadBlock() {
			uint64_t	count=rows-loaded;
			if (count>blocksize) {
				count=blocksize;
			}
			loaded+=count;
			if (count<blocksize) {
				allrows=true;
			}
		}

		uint64_t	rows;
		uint64_t	cols;
		uint64_t	blocksize;
		bool		nevertrue;
		uint64_t	loaded;
		bool		allrows;
		int32_t		dummy;
};

void testBlockCopy(const char *name, uint64_t rows, uint64_t cols,
					uint64_t blocksize, bool nevertrue) {

	stringbuffer	title;
	for (uint16_t i=0; i<3; i++) {

		// copy, assignment from a tablecollection,
		// assignment from a table
		blocktable	src(rows,cols,blocksize,nevertrue);
		table<int32_t>	dst;
		dst.setValue(rows+2,0,-1);
		table<int32_t>	*t=NULL;
		if (!i) {
			t=new table<int32_t>(src);
		} else if (i==1) {
			dst=src;
			t=&dst;
		} else {
			table<int32_t>	tmp(src);
			dst=tmp;
			t=&dst;
		}

		uint64_t	expectedrows=(cols)?rows:0;
		const char	*how=(!i)?"copy":
				(i==1)?"assignment":"table assignment";
		title.clear();
		title.append(name)->append(": ")->append(how);
		title.append(": row count");
		test(title.getString(),t->getRowCount()==expectedrows);

		title.clear();
		title.append(name)->append(": ")->append(how);
		title.append(": col count");
		test(title.getString(),t->getColumnCount()==cols);

		bool	success=true;
		for (uint64_t row=0; row<expectedrows; row++) {
			for (uint64_t col=0; col<cols; col++) {
				if (t->getValue(row,col)!=
						blocktable::value(row,col)) {
					success=false;
				}
			}
		}
		title.clear();
		title.append(name)->append(": ")->append(how);
		title.append(": values");
		test(title.getString(),success);

		if (!i) {
			delete t;
		}
	}
}

void testBlockWrite(const char *name, uint64_t rows, uint64_t cols,
				uint64_t blocksize, bool nevertrue,
				const char *expectedjson,
				const char *expectedindentjson) {

	// build an equivalent table
	table<int32_t>	equiv;
	for (uint64_t col=0; col<cols; col++) {
		equiv.setColumnName(col,blocknames[col]);
	}
	for (uint64_t row=0; row<rows; row++) {
		for (uint64_t col=0; col<cols; col++) {
			equiv.setValue(row,col,blocktable::value(row,col));
		}
	}

	stringbuffer	title;
	stringbuffer	expected;
	stringbuffer	sb;
	for (uint16_t i=0; i<3; i++) {

		// writeJson(), writeJson() with indent, write()
		const char	*how=(!i)?"writeJson":
				(i==1)?"writeJson indent":"write";
		blocktable	src(rows,cols,blocksize,nevertrue);
		expected.clear();
		sb.clear();
		ssize_t	result;
		if (!i) {
			if (expectedjson) {
				expected.append(expectedjson);
			} else {
				equiv.writeJson(&expected,false);
			}
			result=src.writeJson(&sb,false);
		} else if (i==1) {
			if (expectedindentjson) {
				expected.append(expectedindentjson);
			} else {
				equiv.writeJson(&expected,true);
			}
			result=src.writeJson(&sb,true);
		} else {
			if (expectedindentjson) {
				expected.append(expectedindentjson);
			} else {
				equiv.write(&expected);
			}
			result=src.write(&sb);
		}

		title.clear();
		title.append(name)->append(": ")->append(how);
		title.append(": output");
		test(title.getString(),
			!charstring::compare(sb.getString(),
						expected.getString()));

		title.clear();
		title.append(name)->append(": ")->append(how);
		title.append(": return value");
		test(title.getString(),
			result==(ssize_t)sb.getStringLength());
	}
}

// Counts writes and fails call number "failat" and every call after it
// with RESULT_ERROR.  Counts the calls made after the first failure.
class failbuffer : public stringbuffer {
	public:
				failbuffer(size_t failat);
		ssize_t	write(const byte_t *string, size_t size);
		ssize_t	write(const char *string, size_t length);
		size_t	writes;
		size_t	writesafterfail;
	private:
		ssize_t	failWrite(const byte_t *data, size_t size);
		size_t	failat;
};

failbuffer::failbuffer(size_t failat) : stringbuffer() {
	writes=0;
	writesafterfail=0;
	this->failat=failat;
}

ssize_t failbuffer::write(const byte_t *string, size_t size) {
	return failWrite(string,size);
}

ssize_t failbuffer::write(const char *string, size_t length) {
	return failWrite((const byte_t *)string,length);
}

ssize_t failbuffer::failWrite(const byte_t *data, size_t size) {
	writes++;
	if (failat && writes>failat) {
		writesafterfail++;
	}
	if (failat && writes>=failat) {
		return RESULT_ERROR;
	}
	return stringbuffer::write(data,size);
}

// how: 0 is writeJson(), 1 is writeJson() with indent, 2 is write()
static ssize_t writeHow(table<int32_t> *t, output *out, uint16_t how) {
	return (how==2)?t->write(out):t->writeJson(out,how==1);
}

void testFailWrite(uint16_t how) {

	const char	*names[]={"writeJson","writeJson indent","write"};

	// build a table
	table<int32_t>	t;
	for (uint64_t col=0; col<3; col++) {
		t.setColumnName(col,"col");
		for (uint64_t row=0; row<3; row++) {
			t.setValue(row,col,(int32_t)(row*3+col));
		}
	}

	// count the writes of a clean run
	failbuffer	clean(0);
	writeHow(&t,&clean,how);
	size_t	total=clean.writes;

	// fail each of the closing writes
	size_t	closing=(how)?5:2;
	stringbuffer	title;
	for (size_t failat=total-closing+1; failat<=total; failat++) {
		failbuffer	fb(failat);
		ssize_t	result=writeHow(&t,&fb,how);
		title.clear();
		title.append(names[how])->append(": fail write ");
		title.append((uint64_t)failat)->append(" of ");
		title.append((uint64_t)total);
		test(title.getString(),
			result==-1 && fb.writesafterfail==0);
	}
}

int main(int argc, char **argv) {

	header("table");

	uint64_t	rows=10;
	uint64_t	cols=5;
	int32_t	start=-100;

	// integers
	stdoutput.printf("integers, fully populated:\n");

	// fully populate table
	table<int32_t>	intt;
	intt.setCopyColumnNames(true);
	int32_t		count=start;
	for (uint64_t j=0; j<cols; j++) {
		intt.setColumnName(j,colname(j));
	}
	for (uint64_t i=0; i<rows; i++) {
		for (uint64_t j=0; j<cols; j++) {
			intt.setValue(i,j,count);
			count++;
		}
	}

	// test...
	stringbuffer	title;
	count=start;
	test("row count",intt.getRowCount()==(uint64_t)rows);
	test("col count",intt.getColumnCount()==(uint64_t)cols);
	for (uint64_t j=0; j<cols; j++) {
		title.clear();
		title.append("colname(")->append(j)->append(")");
		test(title.getString(),
			!charstring::compare(intt.getColumnName(j),colname(j)));
	}
	for (uint64_t j=cols; j<cols+5; j++) {
		title.clear();
		title.append("colname(")->append(j)->append(") (out of range)");
		test(title.getString(),!intt.getColumnName(j));
	}
	for (uint64_t i=0; i<rows; i++) {
		for (uint64_t j=0; j<cols; j++) {
			title.clear();
			title.append("value(");
			title.append(i);
			title.append(",");
			title.append(j);
			title.append(")");
			test(title.getString(),intt.getValue(i,j)==count);
			count++;
		}
		for (uint64_t j=0; j<5; j++) {
			title.clear();
			title.append("value(");
			title.append(i);
			title.append(",");
			title.append(j);
			title.append(")(out of range)");
			test(title.getString(),!intt.getValue(rows+i,cols+j));
		}
	}
	for (uint64_t i=rows; i<rows+5; i++) {
		title.clear();
		title.append("value(");
		title.append(i);
		title.append(",");
		title.append(0);
		title.append(")(out of range)");
		test(title.getString(),!intt.getValue(rows+i,(uint64_t)0));
	}

	// clear
	intt.clear();
	for (uint64_t j=0; j<5; j++) {
		title.clear();
		title.append("value(");
		title.append(j);
		title.append(",");
		title.append(0);
		title.append(")(out of range after clear)");
		test(title.getString(),!intt.getColumnName(j));
	}
	for (uint64_t j=0; j<5; j++) {
		title.clear();
		title.append("value(");
		title.append(0);
		title.append(",");
		title.append(j);
		title.append(")(out of range after clear)");
		test(title.getString(),!intt.getValue(0,j));
	}
	for (uint64_t i=0; i<5; i++) {
		title.clear();
		title.append("value(");
		title.append(i);
		title.append(",");
		title.append(0);
		title.append(")(out of range after clear)");
		test(title.getString(),!intt.getValue(i,(uint64_t)0));
	}
	stdoutput.printf("\n");


	// copy and assignment
	for (uint16_t i=0; i<2; i++) {

		stdoutput.printf("copy/assignment%s...\n",(!i)?"":" (managed)");

		table<char *>	cch1;
		cch1.setCopyColumnNames(true);
		cch1.setManageArrayValues(i);
		const char *values[]={
			"a","b","c","d","e","f","g","h","i","j","k","l","m",
			"n","o","p","q","r","s","t","u","v","w","x","y","z",
			NULL
		};
		for (uint64_t col=0; col<13; col++) {
			cch1.setColumnName(col,colname(col));
		}
		uint16_t	k=0;
		for (uint64_t row=0; row<2; row++) {
			for (uint64_t col=0; col<13; col++) {
				if (!i) {
					cch1.setValue(row,col,
						(char *)values[k]);
				} else {
					cch1.setValue(row,col,
						charstring::duplicate(
							(char *)values[k]));
				}
				k++;
			}
		}
	
		table<char *>	cch2(cch1);
		for (uint16_t j=0; j<3; j++) {

			// 1st iteration is copy
			// 2nd is assignment
			// 3rd is assignment after clear
			if (j==2) {
				cch2.clear();
			}
			if (j) {
				cch2=cch1;
			}

			// verify flags
			test((!j)?"copy: manage values":
					"assignment: manage values",
						cch2.getManageValues()==
						cch1.getManageValues());
			test((!j)?"copy: manage array values":
					"assignment: manage array values",
						cch2.getManageArrayValues()==
						cch1.getManageArrayValues());

			// verify row/col count
			test((!j)?"copy: row count":"assignment: row count",
							cch2.getRowCount()==
							cch1.getRowCount());
			test((!j)?"copy: col count":"assignment: col count",
							cch2.getColumnCount()==
							cch1.getColumnCount());

			// verify column names
			bool	success=true;
			for (uint64_t col=0; col<13; col++) {
				if (charstring::compare(
						cch1.getColumnName(col),
						cch2.getColumnName(col))) {
					success=false;
					break;
				}
			}
			test((!j)?"copy: columns":"assignment: columns",
								success);

			// verify values
			success=true;
			for (uint64_t row=0; row<2; row++) {
				for (uint64_t col=0; col<13; col++) {
					if (charstring::compare(
						cch1.getValue(row,col),
						cch2.getValue(row,col))) {
						success=false;
						break;
					}
				}
			}
			test((!j)?"copy: values":"assignment: values",success);
		}
	}
	stdoutput.printf("\n");


	// column names
	stdoutput.printf("column names...\n");
	table<int32_t>	cnt;
	cnt.setCopyColumnNames(true);

	// set out of order
	cnt.setColumnName(3,"D");
	test("out of order: col count",cnt.getColumnCount()==4);
	test("out of order: colname(0)",!cnt.getColumnName(0));
	test("out of order: colname(1)",!cnt.getColumnName(1));
	test("out of order: colname(2)",!cnt.getColumnName(2));
	test("out of order: colname(3)",
			!charstring::compare(cnt.getColumnName(3),"D"));
	cnt.setColumnName(1,"B");
	test("out of order: colname(0) after set",!cnt.getColumnName(0));
	test("out of order: colname(1) after set",
			!charstring::compare(cnt.getColumnName(1),"B"));
	test("out of order: colname(2) after set",!cnt.getColumnName(2));
	test("out of order: colname(3) after set",
			!charstring::compare(cnt.getColumnName(3),"D"));
	cnt.setValue(0,3,10);
	test("out of order: value by name",cnt.getValue(0,"D")==10);

	// set to itself
	cnt.setColumnName(3,cnt.getColumnName(3));
	test("set to itself",!charstring::compare(cnt.getColumnName(3),"D"));

	// set again
	cnt.setColumnName(3,"E");
	test("set again",!charstring::compare(cnt.getColumnName(3),"E"));

	// set by setValue only
	cnt.setValue(0,5,20);
	test("set by value: col count",cnt.getColumnCount()==6);
	test("set by value: colname(4)",!cnt.getColumnName(4));
	test("set by value: colname(5)",!cnt.getColumnName(5));
	stdoutput.printf("\n");


	// sparse, managed
	stdoutput.printf("sparse, managed...\n");
	table<char *>	smt;
	smt.setCopyColumnNames(true);
	smt.setManageArrayValues(true);
	smt.setValue(0,0,charstring::duplicate("a"));
	smt.setValue(2,3,charstring::duplicate("b"));
	test("row count",smt.getRowCount()==3);
	test("col count",smt.getColumnCount()==4);
	test("value(0,0)",
		!charstring::compare(smt.getValue(0,(uint64_t)0),"a"));
	test("value(2,3)",!charstring::compare(smt.getValue(2,3),"b"));
	bool	success=true;
	for (uint64_t row=0; row<3; row++) {
		for (uint64_t col=0; col<4; col++) {
			if ((row==0 && col==0) || (row==2 && col==3)) {
				continue;
			}
			if (smt.getValue(row,col)) {
				success=false;
			}
		}
	}
	test("gaps",success);

	// copy
	table<char *>	smt2(smt);
	test("copy: value(0,0)",
		!charstring::compare(smt2.getValue(0,(uint64_t)0),"a"));
	test("copy: value(2,3)",!charstring::compare(smt2.getValue(2,3),"b"));
	test("copy: value(1,1)",!smt2.getValue(1,1));

	// assignment over a populated table
	smt2.setValue(3,0,charstring::duplicate("c"));
	smt2=smt;
	test("assignment: row count",smt2.getRowCount()==3);
	test("assignment: value(2,3)",
			!charstring::compare(smt2.getValue(2,3),"b"));
	test("assignment: value(3,0)",!smt2.getValue(3,(uint64_t)0));

	// clear
	smt.clear();
	test("clear: row count",!smt.getRowCount());
	test("clear: col count",!smt.getColumnCount());
	test("clear: value(0,0)",!smt.getValue(0,(uint64_t)0));
	test("clear: value(2,3)",!smt.getValue(2,3));
	stdoutput.printf("\n");


	// sparse, types without node_zero_value()
	stdoutput.printf("sparse, other types...\n");
	table<bool>	bt;
	bt.setValue(3,3,true);
	test("bool: value(0,0)",!bt.getValue(0,(uint64_t)0));
	test("bool: value(3,2)",!bt.getValue(3,2));
	test("bool: value(3,3)",bt.getValue(3,3));
	table<long long>	llt;
	llt.setValue(3,3,5);
	test("long long: value(0,0)",!llt.getValue(0,(uint64_t)0));
	test("long long: value(3,2)",!llt.getValue(3,2));
	test("long long: value(3,3)",llt.getValue(3,3)==5);
	stdoutput.printf("\n");


	// copy and assignment, managed values
	stdoutput.printf("copy/assignment (managed values)...\n");
	table<int32_t *>	*mvt=new table<int32_t *>();
	mvt->setManageValues(true);
	mvt->setValue(0,0,new int32_t(42));
	mvt->setValue(1,2,new int32_t(43));
	table<int32_t *>	mvt2(*mvt);
	table<int32_t *>	mvt3;
	mvt3.setValue(0,1,NULL);
	mvt3=*mvt;
	for (uint16_t i=0; i<2; i++) {
		table<int32_t *>	&t=(!i)?mvt2:mvt3;
		const char	*title1=(!i)?"copy: manage values":
					"assignment: manage values";
		const char	*title2=(!i)?"copy: manage array values":
					"assignment: manage array values";
		const char	*title3=(!i)?"copy: values duplicated":
					"assignment: values duplicated";
		test(title1,t.getManageValues());
		test(title2,!t.getManageArrayValues());
		test(title3,
			t.getValue(0,(uint64_t)0)!=mvt->getValue(0,(uint64_t)0)
			&& t.getValue(1,2)!=mvt->getValue(1,2));
	}
	delete mvt;
	for (uint16_t i=0; i<2; i++) {
		table<int32_t *>	&t=(!i)?mvt2:mvt3;
		const char	*title1=(!i)?"copy: value(0,0)":
					"assignment: value(0,0)";
		const char	*title2=(!i)?"copy: value(1,2)":
					"assignment: value(1,2)";
		const char	*title3=(!i)?"copy: value(1,1)":
					"assignment: value(1,1)";
		test(title1,*(t.getValue(0,(uint64_t)0))==42);
		test(title2,*(t.getValue(1,2))==43);
		test(title3,!t.getValue(1,1));
	}
	stdoutput.printf("\n");


	// copy and assignment, block-based source
	// (an alarm turns a hang into a failure)
	stdoutput.printf("copy/assignment (block-based source)...\n");
	signalmanager::alarm(5);
	testBlockCopy("never all rows",7,3,3,true);
	testBlockCopy("exact blocks",6,3,3,false);
	testBlockCopy("partial block",7,3,3,false);
	testBlockCopy("zero rows",0,3,3,false);
	testBlockCopy("zero rows, never all rows",0,3,3,true);
	testBlockCopy("zero columns",7,0,3,false);
	testBlockCopy("zero columns, never all rows",7,0,3,true);
	signalmanager::alarm(0);
	stdoutput.printf("\n");

	// write, block-based source
	// (zero columns writes the first block only, as empty rows)
	stdoutput.printf("write (block-based source)...\n");
	signalmanager::alarm(5);
	const char	*zerocolsjson="{\"c\":[],\"r\":[[],[],[]]}";
	const char	*zerocolsindentjson=
				"{\n"
				"\t\"c\": [\n"
				"\t],\n"
				"\t\"r\": [\n"
				"\t\t[\n"
				"\t\t],\n"
				"\t\t[\n"
				"\t\t],\n"
				"\t\t[\n"
				"\t\t]\n"
				"\t]\n"
				"}\n";
	testBlockWrite("never all rows",7,3,3,true,NULL,NULL);
	testBlockWrite("exact blocks",6,3,3,false,NULL,NULL);
	testBlockWrite("partial block",7,3,3,false,NULL,NULL);
	testBlockWrite("zero rows",0,3,3,false,NULL,NULL);
	testBlockWrite("zero rows, never all rows",0,3,3,true,NULL,NULL);
	testBlockWrite("zero columns",7,0,3,false,
				zerocolsjson,zerocolsindentjson);
	testBlockWrite("zero columns, never all rows",7,0,3,true,
				zerocolsjson,zerocolsindentjson);
	signalmanager::alarm(0);
	stdoutput.printf("\n");

	// write errors
	stdoutput.printf("write errors...\n");
	signalmanager::alarm(5);
	for (uint16_t how=0; how<3; how++) {
		testFailWrite(how);
	}
	signalmanager::alarm(0);
	stdoutput.printf("\n");

	return 0;
}
