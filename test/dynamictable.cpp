// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/dynamictable.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include "test.cpp"

char	cn[2];
const char	*colname(int32_t col) {
	cn[0]='A'+(col%26);
	cn[1]='\0';
	return cn;
}

int main(int argc, char **argv) {

	header("dynamictable");

	int32_t	rows=10;
	int32_t	cols=5;
	int32_t	start=-100;

	// integers
	stdoutput.printf("integers, fully populated:\n");

	// fully populate table
	dynamictable<int32_t>	intt;
	int32_t		count=start;
	for (int32_t j=0; j<cols; j++) {
		intt.setColumnName(j,colname(j));
	}
	for (int32_t i=0; i<rows; i++) {
		for (int32_t j=0; j<cols; j++) {
			intt.setValue(i,j,count);
			count++;
		}
	}

	// test...
	stringbuffer	title;
	count=start;
	test("row count",intt.getRowCount()==(uint64_t)rows);
	test("col count",intt.getColCount()==(uint64_t)cols);
	for (int32_t j=0; j<cols; j++) {
		title.clear();
		title.append("colname(")->append(j)->append(")");
		test(title.getString(),
			!charstring::compare(intt.getColumnName(j),colname(j)));
	}
	for (int32_t j=cols; j<cols+5; j++) {
		title.clear();
		title.append("colname(")->append(j)->append(") (out of range)");
		test(title.getString(),!intt.getColumnName(j));
	}
	for (int32_t i=0; i<rows; i++) {
		for (int32_t j=0; j<cols; j++) {
			title.clear();
			title.append("value(");
			title.append(i);
			title.append(",");
			title.append(j);
			title.append(")");
			test(title.getString(),intt.getValue(i,j)==count);
			count++;
		}
		for (int32_t j=0; j<5; j++) {
			title.clear();
			title.append("value(");
			title.append(i);
			title.append(",");
			title.append(j);
			title.append(")(out of range)");
			test(title.getString(),!intt.getValue(rows+i,cols+j));
		}
	}
	for (int32_t i=rows; i<rows+5; i++) {
		title.clear();
		title.append("value(");
		title.append(i);
		title.append(",");
		title.append(0);
		title.append(")(out of range)");
		test(title.getString(),!intt.getValue(rows+i,(int32_t)0));
	}

	// clear
	intt.clear();
	for (int32_t j=0; j<5; j++) {
		title.clear();
		title.append("value(");
		title.append(j);
		title.append(",");
		title.append(0);
		title.append(")(out of range after clear)");
		test(title.getString(),!intt.getColumnName(j));
	}
	for (int32_t j=0; j<5; j++) {
		title.clear();
		title.append("value(");
		title.append(0);
		title.append(",");
		title.append(j);
		title.append(")(out of range after clear)");
		test(title.getString(),!intt.getValue(0,j));
	}
	for (int32_t i=0; i<5; i++) {
		title.clear();
		title.append("value(");
		title.append(i);
		title.append(",");
		title.append(0);
		title.append(")(out of range after clear)");
		test(title.getString(),!intt.getValue(i,(int32_t)0));
	}
}
