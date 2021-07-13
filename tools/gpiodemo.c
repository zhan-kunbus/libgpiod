#include <errno.h>
#include <stdio.h>
#include <gpiod.h>

int main()
{
	struct gpiod_line_request_config config;
	struct timespec ts = { 1, 0 };
	struct gpiod_line_event ev;
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int set_pin = 10;
	int get_pin = 20;
	int evt_pin = 21;
	int ret;

	//open the gpiochip
	chip = gpiod_chip_open("/dev/gpiochip0");
	if (NULL == chip)
		printf("open gpiochip failed\n");

		//set the name of consumer, for display
	config.consumer = "gpiodemo";

	/************ demo for the output pin *******************/
	//get the gpio pin for set
	line = gpiod_chip_get_line(chip, set_pin);
	if (NULL == line)
		printf("get gpio pin for output failed\n");

	//to set the direction as output and default value as 0
	config.request_type = GPIOD_LINE_REQUEST_DIRECTION_OUTPUT;
	config.flags = 0;
	gpiod_line_request(line, &config, 0); //default value as 0

	//to set the value
	ret = gpiod_line_set_value(line, 1);
	if (-1 == ret)
		printf("set value to pin for output failed:%d\n", errno);

	//read to check(only pseudo-,measure the physical pin for real)
	ret = gpiod_line_get_value(line);
	if (-1 == ret)
		printf("get value from pin for output failed:%d\n", errno);
	printf("get value from output pin: %d\n", ret);

	/************ demo for the input pin *******************/
	//get the gpio pin for get
	line = gpiod_chip_get_line(chip, get_pin);
	if (NULL == line)
		printf("get gpio pin for input failed\n");

	//to set the direction as input
	config.request_type = GPIOD_LINE_REQUEST_DIRECTION_INPUT;
	config.flags = 0;
	gpiod_line_request(line, &config, 0);

	//gpiod_line_set_value(line, 1); //set value for input, no effect

	//to read the value
	ret = gpiod_line_get_value(line);
	if (-1 == ret)
		printf("get value from pin for input failed:%d\n", errno);
	printf("get value from input pin: %d\n", ret);

	/************ demo for the event *******************/
	//get the gpio pin for event
	line = gpiod_chip_get_line(chip, evt_pin);
	if (NULL == line)
		printf("get gpio pin for event failed\n");

	//request the edges event
	ret = gpiod_line_request_both_edges_events(line, NULL);
	if (-1 == ret)
		printf("request event failed:%d\n", errno);

	//wait for the event for ts timestamp
	ret = gpiod_line_event_wait(line, &ts);
	if (-1 == ret)
		printf("wait event failed:%d\n", errno);
	if (0 == ret) {
		printf("wait event timeout\n");
		return -1;
	}

	//read the event(1:GPIOD_LINE_EVENT_RISING_EDGE, 2 GPIOD_LINE_EVENT_FALLING_EDGE)
	ret = gpiod_line_event_read(line, &ev);
	if (-1 == ret)
		printf("read event failed:%d\n", errno);
	printf("get event: %d\n", ev.event_type);

	return 0;

}
