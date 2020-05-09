#include "mainForm.h"
#include "studentStorage.h"
#include "studentManagementForm.h"

using namespace std;

MainForm::MainForm(QWidget *parent)
	: QMainWindow(parent),
	  dbConnection(new DatabaseConnection("teacherdb"))
{
	ui.setupUi(this);
	this->showMaximized();
    connect(ui.action_Quit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui.action_Students, SIGNAL(triggered()), this, SLOT(action_StudentsManagement_Click()));

  	try {
		dbConnection->open();
	   	ui.textEdit->setText("Opened database successfully");
		/*StudentStorage studentStorage(*dbConnection);
		if (studentStorage.insertStudent(Student("Mélanie", "L"))) {
			ui.textEdit->setText(ui.textEdit->toPlainText() + "\n" + "Yes");
		}
		else {
			ui.textEdit->setText(ui.textEdit->toPlainText() + "\n" + "No");
		}*/

	}
	catch(runtime_error &err) {
	   ui.textEdit->setText("Can't open database");
	}

	action_StudentsManagement_Click();
}

MainForm::~MainForm()
{
	delete dbConnection;
}
	
void MainForm::action_StudentsManagement_Click()
{
	StudentManagementForm formStudentManagement(this);
	formStudentManagement.setDatabaseConnection(*dbConnection);
	formStudentManagement.exec();
	exit(0);
}



/*int MainForm::callback(void *data, int argc, char **argv, char **azColName)
{

	Ui::MainFormClass *ui = static_cast<Ui::MainFormClass *>(data);
	if (argc>0 && azColName!=nullptr) 
		ui->textEdit->setText(ui->textEdit->toPlainText() + "\n" + argv[1]);
//    int i;
//    fprintf(stderr, "%s: ", (const char*)data);
   
//    for(i = 0; i<argc; i++){
//       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
// 	data
//    printf("\n");
   return 0;
}*/