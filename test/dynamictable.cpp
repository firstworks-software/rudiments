// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/dynamictable.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include "test.cpp"

char	cn[2];
const char	*colname(uint64_t col) {
	cn[0]='A'+(col%26);
	cn[1]='\0';
	return cn;
}

int main(int argc, char **argv) {

	header("dynamictable");

	uint64_t	rows=10;
	uint64_t	cols=5;
	int32_t	start=-100;

	// integers
	stdoutput.printf("integers, fully populated:\n");

	// fully populate table
	dynamictable<int32_t>	intt;
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
	test("col count",intt.getColCount()==(uint64_t)cols);
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

		dynamictable<char *>	cch1;
		cch1.setManageArrayValues(i);
		const char *values[]={
			"a","b","c","d","e","f","g","h","i","j","k","l","m",
			"n","o","p","q","r","s","t","u","v","w","x","y","z",
			NULL
		};
		for (uint64_t col=0; col<13; col++) {
			intt.setColumnName(col,colname(col));
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
	
		dynamictable<char *>	cch2(cch1);
		for (uint16_t j=0; j<3; j++) {

			// 1st iteration is copy
			// 2nd is assignment
			// 3rd is assignment after clear
			if (j==1) {
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
							cch2.getColCount()==
							cch1.getColCount());

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
}
