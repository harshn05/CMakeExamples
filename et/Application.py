
import sys
from PyQt5 import QtCore, QtGui, QtWidgets, uic
from IPython.lib import guisupport
from qtconsole.inprocess import QtInProcessKernelManager
from qtconsole.rich_jupyter_widget import RichJupyterWidget


class QIPythonWidget(RichJupyterWidget):
    def __init__(self, customBanner=None, *args, **kwargs):
        super(QIPythonWidget, self).__init__(*args, **kwargs)
        if customBanner is not None:
            self.banner = customBanner
        self.font_size = 6
        self.kernel_manager = kernel_manager = QtInProcessKernelManager()
        kernel_manager.start_kernel(show_banner=False)
        kernel_manager.kernel.gui = 'qt'
        self.kernel_client = kernel_client = self._kernel_manager.client()
        kernel_client.start_channels()
        def stop():
            kernel_client.stop_channels()
            kernel_manager.shutdown_kernel()
            guisupport.get_app_qt().exit()
        self.exit_requested.connect(stop)
    def push_vars(self, variableDict):
        """
        Given a dictionary containing name / value pairs, push those variables
        to the Jupyter console widget
        """
        self.kernel_manager.kernel.shell.push(variableDict)
    def clear(self):
        """
        Clears the terminal
        """
        self._control.clear()
        # self.kernel_manager
    def print_text(self, text):
        """
        Prints some plain text to the console
        """
        self._append_plain_text(text)
    def execute_command(self, command):
        """
        Execute a command in the frame of the console widget
        """
        self._execute(command, False)
    def pushVariables(self, variableDict):
        """ Given a dictionary containing name / value pairs, push those variables to the IPython console widget """
        self.kernel_manager.kernel.shell.push(variableDict)
    def clearTerminal(self):
        """ Clears the terminal """
        self._control.clear()
    def printText(self, text):
        """ Prints some plain text to the console """
        self._append_plain_text(text)
    def executeCommand(self, command):
        """ Execute a command in the frame of the console widget """
        self._execute(command, False)
        
def print_process_id():
    print(('Process ID is:', os.getpid()))
    
    
class ApplicationGui(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        super(ApplicationGui, self).__init__(parent)
        qtCreatorFile = "Application.ui"  # Enter file here.
        uic.loadUi(qtCreatorFile, self)
        self.show()
        

        self.ipyConsole = QIPythonWidget("Welcome to Application !!!, Powered By:\n\n ")
        self.layoutipy.addWidget(self.ipyConsole)
        self.ipyConsole.clearTerminal()
        # self.ipyConsole.executeCommand("from mylibs import *")   
        self.Dumptoipy()
        
    def Dumptoipy(self):
        self.ipyConsole.pushVariables(dict(self=self))
    def Clearipy(self):
        self.ipyConsole.executeCommand("if 'self' in globals(): del self")
    
if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = ApplicationGui()
    window.show()
    window.setStyle(QtWidgets.QStyleFactory.create('cleanlooks'))
    sys.exit(app.exec_())
