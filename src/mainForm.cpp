#include "ui_mainForm.h"
#include "mainForm.h"
#include <sqlite3.h>

using namespace std;

MainForm::MainForm(QWidget *parent)
	: QMainWindow(parent),
	  dbConnection(new DatabaseConnection("teacherdb"))
{
	ui.setupUi(this);
    connect(ui.action_Quit, SIGNAL(triggered()), this, SLOT(close()));
	sqlite3 *db;
	int rc;

   rc = sqlite3_open(dbConnection->getDbName().c_str(), &db);

   if( rc ) {
	   ui.textEdit->setText("Can't open database");
   } else {
	   ui.textEdit->setText("Opened database successfully");
   }
	//   char *zErrMsg = 0;
   //int rc;
   //string sql;
   //const char* data = "Callback function called";
   //sql = "SELECT * from student";
	//     rc = sqlite3_exec(db, sql.c_str(), callback, (void*)&ui, &zErrMsg);
	//   ui.textEdit->setText(ui.textEdit->toPlainText() + "\n");
   //}
   sqlite3_close(db);
}

MainForm::~MainForm()
{
	delete dbConnection;
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