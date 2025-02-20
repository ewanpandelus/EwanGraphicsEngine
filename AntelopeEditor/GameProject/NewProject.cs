﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Threading.Tasks;
using System.Collections.ObjectModel;
using AntelopeEditor.Utilities;

namespace AntelopeEditor.GameProject
{
    [DataContract]
    public class ProjectTemplate
    {
        [DataMember]
        public string ProjectType { get; set; }

        [DataMember]
        public string ProjectFile { get; set; }
        
        [DataMember]
        public List<string> Folders { get; set; }

        public byte[] Icon { get; set; }

        public byte[] Screenshot { get; set; }

        public string IconFilePath { get; set; }

        public string ScreenshotFilePath { get; set; }

        public string ProjectFilePath { get; set; }
    }

    class NewProject : ViewModelBase
    {
        private ObservableCollection<ProjectTemplate> _projectTemplates = new ObservableCollection<ProjectTemplate>();
        public ReadOnlyObservableCollection<ProjectTemplate> ProjectTemplates 
        { get; }
        public NewProject() 
        {
            ProjectTemplates = new ReadOnlyObservableCollection<ProjectTemplate>(_projectTemplates);

            try
            {
                var templateFiles = Directory.GetFiles(_templatePath, "template.xml", SearchOption.AllDirectories);
                Debug.Assert(templateFiles.Any());
                foreach (var file in templateFiles)
                {
                    var template = Serializer.FromFile<ProjectTemplate>(file);
                    template.IconFilePath = System.IO.Path.GetFullPath(Path.Combine(Path.GetDirectoryName(file), "Icon.png"));
                    template.Icon = File.ReadAllBytes(template.IconFilePath);
                    template.ScreenshotFilePath = System.IO.Path.GetFullPath(Path.Combine(Path.GetDirectoryName(file), "Screenshot.png"));
                    template.Screenshot = File.ReadAllBytes(template.ScreenshotFilePath);
                    template.ProjectFilePath = System.IO.Path.GetFullPath(Path.Combine(Path.GetDirectoryName(file), template.ProjectFile));
                    _projectTemplates.Add(template);
                }
                ValidateProjectPath();
            }
            catch (Exception ex)
            {
                // TODO log error 
                Debug.Write(ex.Message);
                Logger.Log(MessageType.Error, $"Failed to read project templates");
                throw;
            }
        }

        // TODO: use a better path from installation directory
        private readonly string _templatePath = @"..\..\AntelopeEditor\ProjectTemplates";

        private string _projectName = "NewProject";
        public string ProjectName
        {
            get => _projectName;
            set
            {
                if (_projectName != value)
                {
                    _projectName = value;
                    ValidateProjectPath();
                    OnPropertyChanged(nameof(ProjectName));
                }
            }
        }

        private bool _isValid;

        public bool IsValid 
        {
            get => _isValid;
            set 
            {
                if(_isValid != value) 
                {
                    _isValid = value;
                    OnPropertyChanged(nameof(IsValid));
                }
            }
        }

        private string _projectPath = $@"{Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)}\AntelopeProjects\";
        public string ProjectPath
        {
            get => _projectPath;
            set
            {
                if (_projectPath != value)
                {
                    _projectPath = value;
                    ValidateProjectPath();
                    OnPropertyChanged(nameof(ProjectPath));
                }
            }
        }

        private string _errorMsg;
        public string ErrorMsg
        {
            get => _errorMsg;
            set
            {
                if (_errorMsg != value)
                { _errorMsg = value;
                    OnPropertyChanged(nameof(ErrorMsg));
                }
            }
        }

        public string CreateProject(ProjectTemplate template) 
        {
            ValidateProjectPath();
            if (!IsValid) 
            {
                return string.Empty;
            }
            var path = "";
            if (!EndsInDirectorySeparator (ProjectPath)) 
            {
                ProjectPath += @"\";
                path = ProjectPath;
            }
            else 
            {
                path = $@"{ProjectPath}{ProjectName}";
            }
       
            try
            {
                if (!Directory.Exists(path))
                {
                    Directory.CreateDirectory(path);
                }
                foreach (var folder in template.Folders)
                {
                    Directory.CreateDirectory(Path.GetFullPath(Path.Combine(path, folder)));
                }
                var dirInfo = new DirectoryInfo(path + @"\.Antelope\");
                dirInfo.Attributes |= FileAttributes.Hidden;
                File.Copy(template.IconFilePath, Path.GetFullPath(Path.Combine(dirInfo.FullName, "Icon.png")));
                File.Copy(template.ScreenshotFilePath, Path.GetFullPath(Path.Combine(dirInfo.FullName, "Screenshot.png")));

                var projectXml = File.ReadAllText(template.ProjectFilePath);
                projectXml = string.Format(projectXml, ProjectName, ProjectPath);
                var projectPath = Path.GetFullPath(Path.Combine(path, $"{ProjectName}{Project.Extension}"));
                File.WriteAllText(projectPath, projectXml);
                return path;
            }
            catch (Exception ex) 
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, $"Failed to create project {ProjectName}");
                throw;
            }
        }


        private bool ValidateProjectPath()
        {
            var path = ProjectPath;
            if (!EndsInDirectorySeparator(path))
            {
                path += @"\";
            }
            else
            {
                path += $@"{ProjectName}";
                IsValid = false;
                if(string.IsNullOrWhiteSpace(ProjectName.Trim()))
                {
                    ErrorMsg = "Type in a project name.";
                }
                else if (ProjectName.IndexOfAny(Path.GetInvalidFileNameChars()) != -1) 
                {
                    ErrorMsg = "Invalid character(s) used in project name.";
                }
                else if(ProjectPath.IndexOfAny(Path.GetInvalidPathChars()) != -1) 
                {
                    ErrorMsg = "Invalid character(s) used in project path";
                }
                else if (Directory.Exists(path) && Directory.EnumerateFileSystemEntries(path).Any()) 
                {
                    ErrorMsg = "Selected project folder already exists and isn't empty.";
                }
               else 
                { 
                    ErrorMsg = string.Empty;
                    IsValid = true;
                }
            }
            return IsValid;
        }

        bool EndsInDirectorySeparator(string path)
        {
            return path.EndsWith(Path.DirectorySeparatorChar.ToString()) ||
                   path.EndsWith(Path.AltDirectorySeparatorChar.ToString());
        }
    }
}
