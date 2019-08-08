Modoma Forms
===

Applicability
---

The application is written specifically for Modoma Health and Welness Clinic. The goal was to automate the process of customers registration and surveying, including all the related required actions made by the staff of the clinic.
A possibility to edit existing or build new forms is also provided, thus making the application a form/survey builder, serving specific needs of the Clinic.


Technical details
---

#### Languages
This web-site is written in PHP, using Yii framework. For front-end, some Javascript code was written to improve user experience, and, additionally, two Javascript libraries were used. The project also utilizes Bootstrap CSS framework to simplify the task of HTML code styling.

#### Automation
All the most often used tasks were automated and documented with the help of a Makefile, including the project's build and setup. Composer, the most typical PHP packages management tool, is in use too.

#### Databases
API requests to Infusionsoft CRM are made to store and load customers information.
For the rest of the data, MySQL is used.
