# Changelog
All notable changes to this project will be documented in this file.

## [0.0.1]

### Added
- The initial version development (structure and management forms).

## [1.0.0]

### Added
- The edit assessment form as been created.
- The main form now include the assessment list by schools > classes.
- Code coverage of the three library (Models (100%), DataServices (94.8%) and Controllers(100%)) has been improved.
- The project has now 505 unit tests and 5 integrations tests for the database operations.

## [1.1.0]

### Added
- Enforce referential integrity constraint in the different tables.
- Add a version table and class to be able to implement the in-software database upgrade.
- Add the first report Class Assessments Summary Report.

### Changed
- Class form : Ensure the member list resize with the window.
- Assessment Form: let the user choose the max result of the assessment. Not necessarily 100.

### Code related
- Code refactoring.
- Fix compilation warnings in Windows ans OS X.