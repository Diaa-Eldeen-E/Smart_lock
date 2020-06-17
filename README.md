
[![Build Status](https://travis-ci.com/diaa3007/Smart_lock.svg?branch=master)](https://travis-ci.com/diaa3007/Smart_lock/)

### A smart lock system using an LCD and a keypad user interface.

- The user can choose a password of any size
- The password can be changed in two ways:
	- By pressing the button in the Tiva launchpad board the user is prompted to enter the old password and the new password
	- By reprogramming the microcontroller


### It just displays an error message when the password is incorrect but it could be extended to fire an alaram for example by changing the function "incorrect_message" which gets called when the password is wrong

### When the entered password is correct the function "correct_message" gets called, so it could be extended for any application.

