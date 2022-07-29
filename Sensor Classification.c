/************************************************************************************
# Developing Algorithm for automatic cassification of Type S MEMS sensor
-------------------------------------------------------------------------

Goal of the project  was to develop a software solution to automate the classification of
Mems sensor to be deployed in harsh environmental condition.

Based on performance, the sensor is classsified by providing it with a grade.
In special scenarios where the sensor totally underperformes, it is deemed as a REJECT.

Classification Process
----------------------
The classification is done by comparing i/o characteristic of the sensor with a known
and calibrated STANDARD.

The list of input voltage to be applied at the sensor's i/p node stored as a CSV file
is parsed and stored as a const array saved in the PROGRAM space of the ATMEGA micro
controller.

Similarly, the expected output list is stored as well in the PROG space as an array.

*** THE INPUT AND OUTPUT LIST IS INDEX MATCHED

The classification process begins by trigerring a high resolution DAC and providing it with
a 12 bit digital input word. The DAC converts the word into a high res analog value and
applies it to the input of the sensor.

The output for the relevant input is acquired by a 12 bit ADC.

This value is stored in a temp variabe.

This temp variabe is then compared with the correct index from the output list.
The deviation is stored in a variable.

The system iterartes through the entire i/o list and caculates the deviation for each
iteration.

Once all the iterations are complete, the deviation is averaged.
Based on the average, a rating is dispayed.
*********************************************************************************************************/

/**
  * @brief the function iterates through the given data point and compares the system
  * output with a pre-defined data table. The deviation calculated in each iteration
  * is finally averaged and based on it a weight is awarded.
*/

/*
 * The classification chart 1024 data points to iterarte over
*/

#define DATA_POINTS 1024

/*
 * Adding the input data set and storing it in the program memory
*/
const PROGMEM uint16_t ip_dataSet[]={/*<i/p data set 1024 data points>*/};

/*
 * Adding the output data set and storing it in the program memory
*/
const PROGMEM uint16_t op_dataSet[]={/*<o/p data set 1024 data points>*/};


int classification_Algo()
{
	/*
	 * System Check function called to verify hardware connections to the sensor module
	*/
	system_check();
	/*
	 * Begin iteration
	*/
	int deviation=0; //initialize the deviation value to zero
	for(int i=0;i<DATA_POINTS;++i)
	{
		/*
		 * The pgm read pulls data from the PROG space
		 * with proper index by incrementing the pointer
		 * This value is then fed in to the DAC i/p
		 * which o/ps an analog voltage
		*/
		DAC_op(pgm_read_word_near(ip_dataSet+i));
		/*
		 * Delay for reading to settle down
		*/
		delay(200);
		/*
		 * Sensor o/p is stored in a temp variable
		 * which is then compared with the pre-defined
		 * output
		*/
		int sensor_out=sensorRead();
		deviation=deviation + abs(sensor_out-(pgm_read_word_near(op_dataSet+i)));
	}
	/*
	 * Based on certain predefined condition the sensor
	 * is classified into poor, fair and good.
	 * For a poor system function returns 0
	 * For a fair system function returns 1
	 * For a good system function returns 2
	 * Error check doe by a negative return
	*/
	float avg=deviation/DATA_POINTS;
	if(avg<SYS_POOR) return 0;
	else if(avg>SYS_POOR && avg<SYS_FAIR) return 1;
	else if(avg>SYS_FAIR && avg<SYS_GOOD) return 2;
	else return -1;
}
