#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <math.h>


/*
* This program wil cycle through cellular automata
* displaying the result on the screen in a text format.
* This particular program will find the new for bit
* 8, given the old number.
*/

// prototypes
unsigned char neighborhood_bit_7(unsigned char x);
unsigned char neighborhood_bit_0(unsigned char x);
unsigned char cellular_rule(unsigned char x, int rule);
unsigned int neighborhood_bit_1_to_6(unsigned char input, char position);

int main(int argc, char *argv[])
{
	unsigned char x=atoi(argv[1]);
	unsigned char a[8];
	unsigned char c[8];
	unsigned char d=0;
	int i=0;

	printf("x: %d\n",x);
	#pragma omp parallel for reduction(+:result)
		#pragma omp sections [clause...] newline
			private (list)
			firstprivate (list)
			lastprivate (list)
			reduction (operator:list)
			nowait
		#pragma omp section newline
			if (i==0) {
				unsigned char shift_left_1 = 0;
		   	unsigned char shift_right_7 = 0;
				unsigned char neighborhood = 0;
       
				shift_left_1 = (input&3)<<1;
            // store 0 bit in shift_left_3 and
            // shift left to place 3.
       
				shift_right_7 = (input&128)>>7;   
				// shift first two bits to last 2 places
				// and remove all other bits.

				neighborhood = shift_left_1 | shift_right_7;
				// and both parts together to get the
				// places in order of 076, in place
				// 2,1,0. This will now be sent to
				// a function to find the bit value
				// placed in place 7.

				// send value back to main program
				return neighborhood;
				switch (input) {
					 case 7:
						  result = rule_set[7];
						  break;
					 case 6:
						  result = rule_set[6];
						  break;
					 case 5:
						  result = rule_set[5];
						  break;
					 case 4:
						  result = rule_set[4];
						  break;
					 case 3:
						  result = rule_set[3];
						  break;
					 case 2:
						  result = rule_set[2];
						  break;
					 case 1:
						  result = rule_set[1];
						  break;
					 case 0:
						  result = rule_set[0];
						  break;
					 default:
						  //cout<<"Error, bad input, quitting\n";
						  break;
					 }
					 return result;
			result = c[i]*pow(2,i);
			structured_block
		#pragma omp section newline
			structured_block
		#pragma omp section newline
			structured_block


	for (i=0;i<=7;i++){
	if (i==0){
		// calculate neighborhood for bit 0
		a[i] = neighborhood_bit_0(x);
	}
	if (i==7){
		// calculate neighborhood for bit 7
		a[i] = neighborhood_bit_7(x);
	}
	{
		a[i]=neighborhood_bit_1_to_6(x,i);
	}
	}
	 
	// apply the rule to calculate the bit values.
	for (i=0;i<=7;i++){
	  c[i] = cellular_rule(a[i], 195);
	//    printf("bit %d, neighborhood %d,  cellular result: %d\n",i,a[i],c[i]);
	  d += c[i]*pow(2,i);
	}

	printf("\nfinal result: %d\n",d);
	return 0;
}

unsigned char neighborhood_bit_7(unsigned char input)
{
    unsigned char shift_left_2 = 0;
    unsigned char shift_right_6 = 0;
    unsigned char neighborhood = 0;
       
    shift_left_2 = (input&1)<<2;
            // store 0 bit in shift_left_3 and
            // shift left to place 3.
       
    shift_right_6 = (input&192)>>6;   
            // shift first two bits to last 2 places
            // and remove all other bits.
       
    neighborhood = shift_left_2 | shift_right_6;
            // and both parts together to get the
            // places in order of 076, in place
            // 2,1,0.
       
    // send value back to main program
    return neighborhood;
}

unsigned char neighborhood_bit_0(unsigned char input)
{
    unsigned char shift_left_1 = 0;
    unsigned char shift_right_7 = 0;
    unsigned char neighborhood = 0;
       
    shift_left_1 = (input&3)<<1;
            // store 0 bit in shift_left_3 and
            // shift left to place 3.
       
    shift_right_7 = (input&128)>>7;   
            // shift first two bits to last 2 places
            // and remove all other bits.
       
    neighborhood = shift_left_1 | shift_right_7;
            // and both parts together to get the
            // places in order of 076, in place
            // 2,1,0. This will now be sent to
            // a function to find the bit value
            // placed in place 7.
       
    // send value back to main program
    return neighborhood;
}

unsigned int neighborhood_bit_1_to_6(unsigned char input, char position)
{
    unsigned char mask = 0;
    //unsigned char temp = 0;
    //unsigned char shift_right = 0;
    unsigned char neighborhood = 0;
   
    mask = pow(2,position-1)+pow(2,position+1)+pow(2,position);
       
    neighborhood = (input&mask)>>(position-1);
            // store neighborhood in places 2,1,0.
            // get rid of extraneous bits.
           
    // send value back to main program
    return neighborhood;
}

unsigned char cellular_rule(unsigned char input, int rule)
{
    unsigned char result = 0;
    unsigned char rule_set_195[] = {1,1,0,0,0,0,1,1};
    unsigned char rule_set_153[] = {1,0,1,1,0,0,0,1};
    unsigned char rule_set_90[] = {0,1,0,1,1,0,1,0};
    unsigned char rule_set_15[] = {0,0,0,0,1,1,1,1};
    unsigned char rule_set_170[] = {1,0,1,0,1,0,1,0};
    unsigned char rule_set_51[] = {0,0,1,1,0,0,1,1};
    unsigned char rule_set_204[] = {1,1,0,0,0,1,1,0};
	 unsigned char rule_set_150[] = {1,0,0,1,0,1,1,0};
    unsigned char *rule_set;
   
    switch (rule) {
    case 195:
        rule_set=rule_set_195;
        break;
    case 153:
        rule_set=rule_set_153;
        break;
    case 90:
        rule_set=rule_set_90;
        break;
    case 15:
        rule_set=rule_set_15;
        break;
    case 170:
        rule_set=rule_set_170;
        break;
    case 51:
        rule_set=rule_set_51;
        break;
    case 204:
        rule_set=rule_set_204;
        break;
	 case 150:
		 rule_set=rule_set_150;
		 break;
	 }
   
    switch (input) {
    case 7:
        result = rule_set[7];
        break;
    case 6:
        result = rule_set[6];
        break;
    case 5:
        result = rule_set[5];
        break;
    case 4:
        result = rule_set[4];
        break;
    case 3:
        result = rule_set[3];
        break;
    case 2:
        result = rule_set[2];
        break;
    case 1:
        result = rule_set[1];
        break;
    case 0:
        result = rule_set[0];
        break;
    default:
        //cout<<"Error, bad input, quitting\n";
        break;
    }
    return result;
}
