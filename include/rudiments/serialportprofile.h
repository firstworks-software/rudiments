// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERIALPORTPROFILE_H
#define RUDIMENTS_SERIALPORTPROFILE_H

#include <rudiments/private/serialportprofileincludes.h>

// cfmakeraw, cfsetspeed???
// wraps struct termios

/** A serialportprofile is a set of parameters for a serial port such as baud
 *  rate, parity, bits-per character and lots of other settings.  On some
 *  systems, these are referred to as "termios".
 *
 *  In antiquity, teletypes and dumb terminals were the most common
 *  serial-attached devices and they required these to be set to very specific
 *  values.  These days serial ports are more often used for talking to
 *  peripherals on embedded systems but many of the same concepts still
 *  apply. */
class RUDIMENTS_DLLSPEC serialportprofile : public object {
	public:

		enum	inputmode_t {
			cannonical=0,
			raw
		};

		enum	flowcontrol_t {
			fc_none=0,
			fc_software,
			fc_hardware
		};

		enum	baudrate_t {
			baud_0=0,
			baud_50,
			baud_75,
			baud_110,
			baud_134,
			baud_150,
			baud_200,
			baud_300,
			baud_600,
			baud_1200,
			baud_1800,
			baud_2400,
			baud_4800,
			baud_9600,
			baud_19200,
			exta,
			baud_38400,
			extb,
			baud_57600,
			baud_76800,
			baud_115200,
			baud_230400,
			baud_460800,
			baud_500000,
			baud_576000,
			baud_921600,
			baud_1000000,
			baud_1152000,
			baud_1500000,
			baud_2000000,
			baud_2500000,
			baud_3000000,
			baud_3500000,
			baud_4000000
		};

		enum	charsize_t {
			cs_5=0,
			cs_6,
			cs_7,
			cs_8
		};

		enum	newlinedelay_t {
			nl_none=0,
			nl_100
		};

		enum	carriagereturndelay_t {
			cr_none=0,
			cr_depends,
			cr_100,
			cr_150
		};

		enum	tabdelay_t {
			td_0=0,
			td_1,
			td_2,
			td_xtabs
		};

		enum	backspacedelay_t {
			bs_none=0,
			bs_50
		};

		enum	verticaltabdelay_t {
			vt_none=0,
			vt_2
		};

		enum	formfeeddelay_t {
			ff_none=0,
			ff_2
		};

		/** Creates an instance of the serialportprofile class. */
		serialportprofile();

		/** Deletes this instance of the serialportprofile class. */
		~serialportprofile();

		/** Sets the control options to system defaults. */
		void	setDefaultControlOptions();

		/** Sets the local options to system defaults. */
		void	setDefaultLocalOptions();

		/** Sets the input options to system defaults. */
		void	setDefaultInputOptions();

		/** Sets the output options to system defaults. */
		void	setDefaultOutputOptions();

		/** Sets the control characters to system defaults. */
		void	setDefaultControlCharacters();

		/** Sets all options and control
		 *  characters to system defaults. */
		void	setDefaultOptions();

		/** Set control characters to the values pointed to by
		 *  "c_cc".  Note that c_cc must be an array of length
		 *  NCCS. */
		void	setControlCharacters(const byte_t *c_cc);

		/** Serial port configuration requirements are often
		 *  given as a 3 character string representing the
		 *  number of bits-per-char, the parity and the number
		 *  of stop bits such as "8n1", "7e2", etc.
		 *  This methods parses such a string and sets the
		 *  profile to match those values. */
		void	parseOptionsString(const char *string);

		/** Set input mode to "inputmode". */
		void		setInputMode(inputmode_t inputmode);

		/** Get the current input mode setting. */
		inputmode_t	getInputMode();

		/** Set flow control to "flowcontrol". */
		void		setFlowControl(flowcontrol_t flowcontrol);

		/** Get the current flow control setting. */
		flowcontrol_t	getFlowControl();

		/** Sets the (input and output) baud rate.  Default is 0. */
		void	setBaud(const char *baudrate);

		/** Sets the (input and output) baud rate.  Default is 0. */
		void	setBaud(baudrate_t baudrate);

		/** Sets the input baud rate.  Default is 0. */
		bool	setInputBaud(const char *baudrate);

		/** Sets the input baud rate.  Default is 0. */
		bool	setInputBaud(baudrate_t baudrate);

		/** Sets the output baud rate.  Default is 0. */
		bool	setOutputBaud(const char *baudrate);

		/** Sets the output baud rate.  Default is 0. */
		bool	setOutputBaud(baudrate_t baudrate);

		/** Set the character size.  Default is 5 bits.
		 *  Corresponding termios flag: CSIZE. */
		void	setCharacterSize(charsize_t size);

		/** Use two stop bits if "truefalse" is true
		 *  or one stop bit if "truefalse" is false.
		 *  Default is one stop bit.
		 *  Corresponding termios flag: CSTOPB. */
		void	setTwoStopBits(bool truefalse);

		/** Turns the receiver on or off.
		 *  (Enables serial port to read incoming data or not.)
		 *  Default is off.
		 *  (You probably always want to set this on.)
		 *  Corresponding termios flag: CREAD. */
		void	setReceiverOn(bool truefalse);

		/** Turns parity checking on or off.
		 *  Default is off.
		 *  Corresponding termios flag: CPARENB. */
		void	setParityCheck(bool truefalse);

		/** Use odd parity if "truefalse" is true
		 *  or even parity if "truefalse" is false.
		 *  Default is even parity.
		 *  Corresponding termios flag: PARODD. */
		void	setOddParity(bool truefalse);

		/** Lower modem control lines after
		 *  last process closes the device (hang up).
		 *  Default is not to hang up on close.
		 *  Corresponding termios flag: HUPCL. */
		void	setHangupOnClose(bool truefalse);

		/** Ignore (possibly sporadic) job control and hangup
		 *  signals that could terminate or otherwise control
		 *  the program.
		 *  Default is not to ignore.
		 *  (This should be set to true when communicating
		 *  with a device connected directly to the computer as
		 *  opposed to over a modem.)
		 *  Corresponding termios flag: CLOCAL. */
		void	setIgnoreModemControlLines(bool truefalse);

		/** Block output from a nonconcurrent shell layer.
		 *  Default is not to block.
		 *  Corresponding termios flag: LOBLK. */
		void	setBlockJobControlOutput(bool truefalse);

		/** Use RTS/CTS flow control.
		 *  Default is not to use RTS/CTS flow control.
		 *  Corresponding termios flags:
		 *  CRTSCTS/NEW_CRTSCTS/CRTS_IFLOW/CRTS_OFLOW. */
		void	setHardwareFlowControl(bool truefalse);

		/** Returns current (input and output) baud. */
		baudrate_t	getBaud();

		/** Returns current input baud. */
		baudrate_t	getInputBaud();

		/** Returns current output baud. */
		baudrate_t	getOutputBaud();

		/** Returns current character size. */
		charsize_t	getCharacterSize();

		/** Returns true if two stop bits are being used. */
		bool		getTwoStopBits();

		/** Returns true if the receiver is on. */
		bool		getReceiverOn();

		/** Returns true if parity check is enabled. */
		bool		getParityCheck();

		/** Returns true if odd parity is being used. */
		bool		getOddParity();

		/** Returns true if the modem lines will be
		 *  lowered when the last device closes the
		 *  device. */
		bool		getHangupOnClose();

		/** Returns true if (possibly sporadic) job
		 *  control and hangup signals are being ignored. */
		bool		getIgnoreModemControlLines();

		/** Returns true if output from a nonconcurrent
		 *  shell layer is being blocked. */
		bool		getBlockJobControlOutput();

		/** Returns true if RTS/CTS hardware flow
		 *  control is being used. */
		bool		getHardwareFlowControl();

		/** Cause corresponding signals to be raised when any of
		 *  the INTR, QUIT, SUSP or DSUSP characters are received
		 *  default is not to raise signals
		 *  Corresponding termios flag: ISIG. */
		void	setGenerateSignals(bool truefalse);

		/** Enable special characters EOF, EOL, EOL2, ERASE,
		 *  KILL, LNEXT, REPRINT, STATUS and WERASE, buffer by
		 *  lines.
		 *  Default is not to enable the special characters and
		 *  not buffer by lines.
		 *  Corresponding termios flag: ICANON. */
		void	setCanonicalInput(bool truefalse);

		/** If cannonicalInput() is also set true, converts
		 *  characters from input to lower case unless they are
		 *  preceeded by a \.  On output, preceeds each upper
		 *  case character with a \ and converts all lower case
		 *  characters to uppercase.
		 *  Default is not to use escaped upper case.
		 *  (This is used when communicating with terminals that
		 *  can display upper or lower case, but only transmit and
		 *  receive upper case.)
		 *  Corresponding termios flag: XCASE. */
		void	setEscapedUpperCase(bool truefalse);

		/** If cannonicalInput() is also set true then echo input.
		 *  Corresponding termios flag: ECHO. */
		void	setEchoInput(bool truefalse);

		/** If cannonicalInput() is also set true, character
		 *  set using eraseCharacter() erases the preceeding
		 *  character and character set using
		 *  wordEraseCharacter() erases the preceeding word.
		 *  Corresponding termios flag: ECHOE. */
		void	setEraseCharactersOn(bool truefalse);

		/** If cannonicalInput() is also set true, character
		 *  set using killCharacter() erases the current line.
		 *  Corresponding termios flag: ECHOK. */
		void	setKillCharacterOn(bool truefalse);

		/** If cannonicalInput() is also set true, new line
		 *  characters are echoed even if echoInput() is set false.
		 *  Corresponding termios flag: ECHONL. */
		void	setEchoNewLine(bool truefalse);

		/** If cannonicalInput() is also set true, enables
		 *  characters set by secondEndOfLineCharacter(),
		 *  reprintCharacter() and wordEraseCharacter().  Also
		 *  enables lowerCase().
		 *  Corresponding termios flag: IEXTEN. */
		void	setExtendedFunctions(bool truefalse);

		/** If echoInput() is also set true, control characters
		 *  are echoed as ^X where X is the ascii code for the
		 *  character plus 0x40.
		 *  Corresponding termios flag: ECHOCTL. */
		void	setEchoControlCharacters(bool truefalse);

		/** If cannonicalInput() and echoInput() are also set
		 *  true, characters are printed as they are erased.
		 *  Corresponding termios flag: ECHOPRT. */
		void	setEchoErasedCharacter(bool truefalse);

		/** If cannonicalInput() is also set, the character set
		 *  using killCharacter() causes the line to be erased
		 *  by erasing each character on the line.
		 *  (Useful when a terminal doesn't support the KILL
		 *  character but does support the ERASE character.)
		 *  Corresponding termios flag: ECHOKE. */
		void	setEmulateKill(bool truefalse);

		/** Disables flushing of the input/output queues when
		 *  generating SIGINT, SIGQUIT or SIGSUSP signals.
		 *  Corresponding termios flag: NOFLSH. */
		void	setNoFlushAfterInterruptOrQuit(bool truefalse);

		/** All characters in the input queue are reprinted when
		 *  the next character is read.
		 *  Corresponding termios flag: PENDIN. */
		void	setRetypePendingCharacters(bool truefalse);

		/** Send the SIGTTOU signal to the process group of a
		 *  background process which tries to write to its
		 *  controlling terminal.
		 *  Corresponding termios flag: TOSTOP. */
		void	setSendSignalForBackgroundOutput(bool truefalse);

		/** Returns true if corresponding signals will be raised
		 *  when any of the INTR, QUIT, SUSP or DSUSP characters
		 *  are received default is not to raise signals. */
		bool	getGenerateSignals();

		/** Returns true if special characters EOF, EOL, EOL2,
		 *  ERASE, KILL, LNEXT, REPRINT, STATUS and WERASE are
		 *  enabled and buffering is being done by lines. */
		bool	getCanonicalInput();

		/** Returns true if on input, characters are being
		 *  converted from upper to lower case unless they are
		 *  preceeded by a \ and on output lower case characters
		 *  are being converted to upper case and upper case
		 *  characters are being preceeded with a \. */
		bool	getEscapedUpperCase();

		/** Returns true if input characters are being echoed. */
		bool	getEchoInput();

		/** Returns true if the character set using
		 *  eraseCharacter() erases the preceeding
		 *  character and the character set using
		 *  wordEraseCharacter() erases the preceeding word. */
		bool	getEraseCharactersOn();

		/** Returns true if the character set using
		 *  killCharacter() erases the current line. */
		bool	getKillCharacterOn();

		/** Returns true if new line characters are being echoed
		 *  even if echoInput() is set false. */
		bool	getEchoNewLine();

		/** Returns true if characters set by
		 *  secondEndOfLineCharacter(), reprintCharacter() and
		 *  wordEraseCharacter() are enabled and lowerCase() is
		 *  enabled. */
		bool	getExtendedFunctions();

		/** Returns true if control characters are being echoed
		 *  as ^X where X is the ascii code for the character
		 *  plus 0x40. */
		bool	getEchoControlCharacters();

		/** Returns true if characters are being printed as they
		 *  are erased. */
		bool	getEchoErasedCharacter();

		/** Returns true if the character set using
		 *  killCharacter() causes the line to be erased
		 *  by erasing each character on the line. */
		bool	getEmulateKill();

		/** Returns true if flushing of the input/output queues
		 *  when generating SIGINT, SIGQUIT or SIGSUSP signals
		 *  is enabled. */
		bool	getNoFlushAfterInterruptOrQuit();

		/** Returns true if all characters in the input queue
		 *  are reprinted when the next character is read. */
		bool	getRetypePendingCharacters();

		/** Returns true if the SIGTTOU signal is being sent
		 *  to the process group of a background process which
		 *  tries to write to its controlling terminal. */
		bool	getSendSignalForBackgroundOutput();

		/** Enable parity checking on input
		 *  Corresponding termios flag: INPCK. */
		void	setInputParityCheck(bool truefalse);

		/** Ignore parity errors (ie. if a character has a
		 *  parity error, just return what we got, rather than
		 *  marking or converting it).
		 *  Corresponding termios flag: IGNPAR. */
		void	setIgnoreParityErrors(bool truefalse);

		/** Unless ignoreParityErrors() is set true, prefix a
		 *  character with a parity error with \337 \0.  The
		 *  default is to convert it to \0 unless
		 *  ignoreParityErrors() is set true.
		 *  Corresponding termios flag: PARMRK. */
		void	setMarkParityErrors(bool truefalse);

		/** Set the 8th bit of each character (the parity bit) to 0.
		 *  Corresponding termios flag: ISTRIP. */
		void	setStripParityBits(bool truefalse);

		/** Enable XON/XOFF flow control on output.
		 *  Corresponding termios flag: IXON. */
		void	setSoftwareFlowControlOnOutput(bool truefalse);

		/** Enable XON/XOFF flow control on input.
		 *  Corresponding termios flag: IXOFF. */
		void	setSoftwareFlowControlOnInput(bool truefalse);

		/** Enable any character to restart output.
		 *  Corresponding termios flag: IXANY. */
		void	setAnyCharacterStartsFlow(bool truefalse);

		/** Ignore BREAK character. 
		 *  Corresponding termios flag: IGNBRK. */
		void	setIgnoreBreak(bool truefalse);

		/** If ignoreBreak() isn't set true and a BREAK character
		 *  is received, flush input and output queues and send a
		 *  SIGINT if the serial port is the controlling terminal
		 *  of the process group.  If ignoreBreak() is not set,
		 *  the default is to return a \0 character or \377 \0
		 *  if markParityErrors() is set true.
		 *  Corresponding termios flag: BRKINT. */
		void	setSendSignalOnBreak(bool truefalse);

		/** Translate new line to carriage return on input
		 *  termios flag: INLCR. */
		void	setMapNewLineToCarriageReturnOnInput(bool truefalse);

		/** ??? 
		 *  Corresponding termios flag: ONOEOT. */
		void	setDiscardEndOfTransmission(bool truefalse);

		/** Ignore carriage return on input.
		 *  Corresponding termios flag: IGNCR. */
		void	setIgnoreCarriageReturn(bool truefalse);

		/** Translate carriage return to new line on input.
		 *  Corresponding termios flag: ICRNL. */
		void	setMapCarriageReturnToNewLineOnInput(bool truefalse);

		/** Map uppercase characters to lowercase on input.
		 *  Corresponding termios flag: IUCLC. */
		void	setLowerCase(bool truefalse);

		/** Ring bell when input queue is full.
		 *  Corresponding termios flag: IMAXBEL. */
		void	setBellIfLineTooLong(bool truefalse);

		/** Returns true if input parity checking is enabled. */
		bool	getInputParityCheck();

		/** Returns true if parity errors are being ignored. */
		bool	getIgnoreParityErrors();

		/** Returns true if characters with parity errors are
		 *  prefixed with \377 \0. */
		bool	getMarkParityErrors();

		/** Returns true if the 8th bit (the parity bit) of
		 *  each character is being set to 0. */
		bool	getStripParityBits();

		/** Returns true if XON/XOFF flow control is enabled on
		 *  output. */
		bool	getSoftwareFlowControlOnOutput();

		/** Returns true if XON/XOFF flow control is enabled on
		 *  input. */
		bool	getSoftwareFlowControlOnInput();

		/** Returns true if any character will restart output. */
		bool	getAnyCharacterStartsFlow();

		/** Returns true if the BREAK character is being ignored. */
		bool	getIgnoreBreak();

		/** Returns true if input and output queues are flushed
		 *  and a SIGINT is sent to the process group if the
		 *  serial port is the controlling terminal when a BREAK
		 *  character is received. */
		bool	getSendSignalOnBreak();

		/** Returns true if new lines are mapped to carriage
		 *  returns on input. */
		bool	getMapNewLineToCarriageReturnOnInput();

		/** ??? */
		bool	getDiscardEndOfTransmission();

		/** Returns true if carriage returns are ignored on input. */
		bool	getIgnoreCarriageReturn();

		/** Returns true if carriage returns are mapped to new
		 *  lines on input. */
		bool	getMapCarriageReturnToNewLineOnInput();

		/** Returns true if uppercase characters are mapped to
		 *  lowercase on input. */
		bool	getLowerCase();

		/** Returns true if the bell will be rung when the input
		 *  queue is full. */
		bool	getBellIfLineTooLong();


		/** Enables implementation-defined output processing.
		 *  Corresponding termios flag: OPOST. */
		void	setPostProcessOutput(bool truefalse);

		/** Map lowercase characters to uppercase on output.
		 *  Corresponding termios flag: OLCUC. */
		void	setOutputUpperCase(bool truefalse);

		/** Map new line to carriage return/new line on output.
		 *  Corresponding termios flag: ONLCR. */
		void	setMapNewLineToCarriageReturnNewLineOnOutput(
							bool truefalse);

		/** Map carriage return to new line on output.
		 *  Corresponding termios flag: OCRNL. */
		void	setMapCarriageReturnToNewLineOnOutput(bool truefalse);

		/** Don't output carriage return at column 0.
		 *  Corresponding termios flag: ONOCR. */
		void	setDontOutputCarriageReturnAtColumnZero(bool truefalse);

		/** Map new line to carriage return on output.
		 *  Corresponding termios flag: ONLRET. */
		void	setMapNewLineToCarriageReturnOnOutput(bool truefalse);

		/** Send fill characters for delay instead of using a
		 *  timed delay.
		 *  Corresponding termios flag: OFILL. */
		void	setUseFillCharactersForDelay(bool truefalse);

		/** Use the DEL character instead of NULL for the fill
		 *  character.
		 *  Corresponding termios flag: OFDEL. */
		void	setUseDelForFill(bool truefalse);

		/** Map tabs to spaces. 
		 *  Corresponding termios flag: XTAGS/OXTABS/TAB3. */
		void	setExpandTabToSpaces(bool truefalse);

		/** Send a delay after each new line character.
		 *  Corresponding termios flag: NLDLY. */
		void	setDelayAfterNewLine(newlinedelay_t nldelay);

		/** Send a delay after each carriage return character.
		 *  Corresponding termios flag: CRDLY. */
		void	setDelayAfterCarriageReturn(
					carriagereturndelay_t crdelay);

		/** Send a delay after each tab character
		 *  Corresponding termios flag: TABDLY. */
		void	setDelayAfterTab(tabdelay_t tabdelay);

		/** Send a delay after each backspace character
		 *  Corresponding termios flag: BSDLY. */
		void	setDelayAfterBackSpace(backspacedelay_t bsdelay);

		/** Send a delay after each vertical tab character.
		 *  Corresponding termios flag: VTDLY. */
		void	setDelayAfterVerticalTab(verticaltabdelay_t vtdelay);

		/** Send a delay after each form feed character
		 *  Corresponding termios flag: FFDLY. */
		void	setDelayAfterFormFeed(formfeeddelay_t ffdelay);

		/** Returns true if implementation-defined output
		 *  processing is enabled. */
		bool	getPostProcessOutput();

		/** Returns true if lowercase characters are mapped to
		 *  uppercase on output. */
		bool	getOutputUpperCase();

		/** Returns true if new lines are mapped to carriage
		 *  return/new line on output. */
		bool	getMapNewLineToCarriageReturnNewLineOnOutput();

		/** Returns true if carriage returns are mapped to new
		 *  lines on output. */
		bool	getMapCarriageReturnToNewLineOnOutput();

		/** Returns true if carriage returns aren't sent at
		 *  column 0. */
		bool	getDontOutputCarriageReturnAtColumnZero();

		/** Returns true if new lines are mapped to carriage
		 *  returns on output. */
		bool	getMapNewLineToCarriageReturnOnOutput();

		/** Returns true if fill characters are sent for delay
		 *  instead of using a timed delay. */
		bool	getUseFillCharactersForDelay();

		/** Returns true if the DEL character is used instead of
		 *  NULL for the fill character. */
		bool	getUseDelForFill();

		/** Returns true if the tabs are mapped to spaces. */
		bool	getExpandTabToSpaces();

		/** Returns the delay that is sent after new line
		 *  characters. */
		newlinedelay_t		getDelayAfterNewLine();

		/** Returns the delay that is sent after carriage return
		 *  characters. */
		carriagereturndelay_t	getDelayAfterCarriageReturn();

		/** Returns the delay that is sent after tab characters. */
		tabdelay_t		getDelayAfterTab();

		/** Returns the delay that is sent after backspace
		 *  characters. */
		backspacedelay_t	getDelayAfterBackSpace();

		/** Returns the delay that is sent after vertical tab
		 *  characters. */
		verticaltabdelay_t	getDelayAfterVerticalTab();

		/** Returns the delay that is sent after form feed
		 *  characters. */
		formfeeddelay_t		getDelayAfterFormFeed();

		/** Sets the character that will cause a SIGINT to be
		 *  sent to the process when generateSignals() is set
		 *  true.
		 *  Corresponding termios flag: VINTR. */
		void	setInterruptCharacter(byte_t character);

		/** Sets the character that will cause a SIGQUIT to be
		 *  sent to the process when generateSignals() is set
		 *  true.
		 *  Corresponding termios flag: VQUIT. */
		void	setQuitCharacter(byte_t character);

		/** Sets the character that will cause a character erase
		 *  when canonicalInput() is set to true
		 *  Corresponding termios flag: VERASE. */
		void	setEraseCharacter(byte_t character);

		/** Sets the character that will cause a line erase
		 *  when canonicalInput() is set to true
		 *  Corresponding termios flag: VKILL. */
		void	setKillCharacter(byte_t character);

		/** Sets the character that will cause the pending tty
		 *  buffer to be sent to the program without waiting for
		 *  end-of-line and read()'s to return 0 when
		 *  canonicalInput() is set to true
		 *  Corresponding termios flag: VEOF. */
		void	setEndOfFileCharacter(byte_t character);

		/** Sets the end-of-line character, recognized when 
		 *  canonicalInput() is set to true
		 *  Corresponding termios flag: VEOL. */
		void	setEndOfLineCharacter(byte_t character);

		/** Sets the "other" end-of-line character, recognized
		 *  when canonicalInput() is set to true
		 *  Corresponding termios flag: VEOL2. */
		void	setSecondEndOfLineCharacter(byte_t character);

		/** Sets the switch character.
		 *  Corresponding termios flag: VSWTCH/VSWTC. */
		void	setSwitchCharacer(byte_t character);

		/** Sets the start character for XON/XOFF flow control
		 *  Corresponding termios flag: VSTART. */
		void	setStartCharacter(byte_t character);

		/** Sets the stop character for XON/XOFF flow control
		 *  Corresponding termios flag: VSTOP. */
		void	setStopCharacter(byte_t character);

		/** Sets the character that will cause a SIGSUSP to be
		 *  sent to the process when generateSignals() is set
		 *  true.
		 *  Corresponding termios flag: VSUSP. */
		void	setSuspendCharacter(byte_t character);

		/** Sets the character that will cause a SIGTSTP to be
		 *  sent to the process when generateSignals() and
		 *  extendedFunctions() are set true.
		 *  Corresponding termios flag: VDSUSP. */
		void	setDelayedSuspendCharacter(byte_t character);

		/** Sets the character that "quotes" the next character,
		 *  depriving it of special meaning, recognized when
		 *  extendedFunctions() is set true.
		 *  Corresponding termios flag: VLNEXT. */
		void	setLiteralNextCharcter(byte_t character);

		/** Sets the word erase character, recognized when
		 *  canonicalInput() and extendedFunctions() are set true.
		 *  Corresponding termios flag: VWERASE. */
		void	setWordEraseCharcter(byte_t character);

		/** Sets the character that causes unread characters to
		 *  be reprinted, recognized when canonicalInput() and
		 *  extendedFunctions() are set true.
		 *  Corresponding termios flag: VREPRINT. */
		void	setReprintCharacter(byte_t character);

		/** Sets the character that toggles discarding pending
		 *  output, recognized when extendedFunctions() is set
		 *  true.
		 *  Corresponding termios flag: VDISCARD. */
		void	setDiscardPendingOutputCharacter(byte_t character);

		/** Sets the status request character.
		 *  Corresponding termios flag: VSTATUS. */
		void	setStatusRequestCharacter(byte_t character);

		/** Sets the number of characters that must be read
		 *  before a read() will begin waiting for readTimeout()
		 *  deciseconds before falling through.
		 *  Corresponding termios flag: VMIN. */
		void	setReadThreshold(byte_t count);

		/** Sets the number of deciseconds that a read() will
		 *  wait after reading readThreshold() characters before
		 *  falling through.
		 *  Corresponding termios flag: VTIME. */
		void	setReadTimeout(byte_t deciseconds);


		/** Returns the character that will cause a SIGINT to be
		 *  sent to the process when generateSignals() is set
		 *  true. */
		byte_t	getInterruptCharacter();

		/** Returns the character that will cause a SIGQUIT to be
		 *  sent to the process when generateSignals() is set
		 *  true. */
		byte_t	getQuitCharacter();

		/** Returns the character that will cause a character
		 *  erase when canonicalInput() is set to true. */
		byte_t	getEraseCharacter();

		/** Returns the character that will cause a line erase
		 *  when canonicalInput() is set to true. */
		byte_t	getKillCharacter();

		/** Returns the character that will cause the pending tty
		 *  buffer to be sent to the program without waiting for
		 *  end-of-line and read()'s to return 0 when
		 *  canonicalInput() is set to true. */
		byte_t	getEndOfFileCharacter();

		/** Returns the end-of-line character, recognized when 
		 *  canonicalInput() is set to true. */
		byte_t	getEndOfLineCharacter();

		/** Returns the "other" end-of-line character, recognized
		 *  when canonicalInput() is set to true. */
		byte_t	getSecondEndOfLineCharacter();

		/** Returns the switch character. */
		byte_t	getSwitchCharacer();

		/** Returns the start character for XON/XOFF flow control. */
		byte_t	getStartCharacter();

		/** Returns the stop character for XON/XOFF flow control. */
		byte_t	getStopCharacter();

		/** Returns the character that will cause a SIGSUSP to be
		 *  sent to the process when generateSignals() is set
		 *  true. */
		byte_t	getSuspendCharacter();

		/** Returns the character that will cause a SIGTSTP to be
		 *  sent to the process when generateSignals() and
		 *  extendedFunctions() are set true. */
		byte_t	getDelayedSuspendCharacter();

		/** Returns the character that "quotes" the next
		 *  character, depriving it of special meaning,
		 *  recognized when extendedFunctions() is set true. */
		byte_t	getLiteralNextCharcter();

		/** Returns the word erase character, recognized when
		 *  canonicalInput() and extendedFunctions() are set true. */
		byte_t	getWordEraseCharcter();


		/** Returns the character that causes unread characters
		 *  to be reprinted, recognized when canonicalInput() and
		 *  extendedFunctions() are set true. */
		byte_t	getReprintCharacter();

		/** Returns the character that toggles discarding pending
		 *  output, recognized when extendedFunctions() is set
		 *  true. */
		byte_t	getDiscardPendingOutputCharacter();

		/** Returns the status request character. */
		byte_t	getStatusRequestCharacter();

		/** Returns the number of characters that must be read
		 *  before a read() will begin waiting for readTimeout()
		 *  deciseconds before falling through. */
		byte_t	getReadThreshold();

		/** Returns the number of deciseconds that a read() will
		 *  wait after reading readThreshold() characters before
		 *  falling through. */
		byte_t	getReadTimeout();

	#include <rudiments/private/serialportprofile.h>
};

#endif
