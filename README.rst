==================
Arduino UDOO rover
==================

Arduino rover with MC33926 motor driver shield powered by UDOO prototyping board.

ADK 2012 compatibility
----------------------

The main feature of this sketch is that it's ADK enabled. In the main loop Arduino waits for any commands sent by an Android device. You can find an `Android application`_, ready to deploy, which is fully compatible with this sketch.


.. _Android application: https://github.com/palazzem/android-udoo-rover

Documentation
-------------

With this Arduino sketch you can control two DC motors using a motor driver.
This function can be used to send commands to Arduino :

.. code-block:: c

	void jeepCommandInterpreter(uint8_t commandMovement, unsigned int commandSpeed);

``commandMovement`` defines where the rover should go according to this switch case:

.. code-block:: c

	case 0:
      Serial.println("Received command: 0 -> move forward");
      goForward(vPower);
      break;
    case 1:
      Serial.println("Received command: 1 -> move backward");
      goBackward(vPower);
      break;
    case 2:
      Serial.println("Received command: 2 -> turn left");
      turnLeft(vPower);
      break;
    case 3:
      Serial.println("Received command: 3 -> turn right");
      turnRight(vPower);
      break;
    case 4:
      Serial.println("Received command: 4 -> turn back");
      turnBack(vPower);
      break;

You can use command ``5`` to test all rover movements.

Protection
----------

**WARNING**: to avoid any damage to your DC motors, be sure to edit defined ``MAX_POWER``. This variable is used by ``motorProtection`` which is executed before launch any commands to motor driver.

License
-------

FreeBSD (see ``LICENSE`` file)
