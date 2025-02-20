using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace AntelopeEditor.GameProject
{
    /// <summary>
    /// Interaction logic for NewProjectView.xaml
    /// </summary>
    public partial class NewProjectView : UserControl
    {
        public NewProjectView()
        {
            InitializeComponent();
        }

        private void OnCreate_Button_Click(object sender, RoutedEventArgs e) 
        {
            var vm = DataContext as NewProject;
            var projectpath = vm.CreateProject(templateListBox.SelectedItem as ProjectTemplate);
            bool dialogResult = false;
            var win = Window.GetWindow(this);
            if (!string.IsNullOrEmpty(projectpath))
            {
                dialogResult = true;
                var project = OpenProject.Open(new ProjectData() { ProjectName = vm.ProjectName, ProjectPath = vm.ProjectPath });
                win.DataContext = project;
            }
            win.DialogResult = dialogResult;
            win.Close();  
        }
    }
}
