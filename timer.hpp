#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {

	public : 
	Timer(bool initRunning = false);
	
	virtual ~Timer();
	
	int getElapsedTime() const;
	bool isRunning()const;
	
	void start();
	void stop();
	
	void restart(bool continueRunning = false);

	private :
	sf::Clock clock_;
	int buffer_;
	bool running_;


};

#endif
