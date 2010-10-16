namespace Treebuilder
{
    partial class Form1
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("root");
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.textBoxtxtPrefix = new System.Windows.Forms.TextBox();
            this.tbdef1 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.tbmen1 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonGenerate = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.buttonLoad = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.buttonSave = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.textBoxParValDefault = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.textBoxParValLower = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.textBoxParValUpper = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.textBoxInfo = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxNodeName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxResult = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.buttonRename = new System.Windows.Forms.Button();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.buttonDelete = new System.Windows.Forms.Button();
            this.comboBoxParType = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.checkBoxCreateEnum = new System.Windows.Forms.CheckBox();
            this.buttonAdd = new System.Windows.Forms.Button();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "N.png");
            this.imageList1.Images.SetKeyName(1, "B.png");
            this.imageList1.Images.SetKeyName(2, "M.png");
            this.imageList1.Images.SetKeyName(3, "P.png");
            this.imageList1.Images.SetKeyName(4, "A.png");
            // 
            // textBoxtxtPrefix
            // 
            this.textBoxtxtPrefix.AcceptsTab = true;
            this.textBoxtxtPrefix.Location = new System.Drawing.Point(308, 131);
            this.textBoxtxtPrefix.Name = "textBoxtxtPrefix";
            this.textBoxtxtPrefix.Size = new System.Drawing.Size(29, 20);
            this.textBoxtxtPrefix.TabIndex = 8;
            this.textBoxtxtPrefix.TabStop = false;
            this.textBoxtxtPrefix.Text = "txt";
            this.textBoxtxtPrefix.WordWrap = false;
            // 
            // tbdef1
            // 
            this.tbdef1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbdef1.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbdef1.Location = new System.Drawing.Point(308, 160);
            this.tbdef1.Name = "tbdef1";
            this.tbdef1.Size = new System.Drawing.Size(778, 22);
            this.tbdef1.TabIndex = 8;
            this.tbdef1.TabStop = false;
            this.tbdef1.Text = "char\\t\\m[] \\tPROGMEM=\"\\s\";";
            this.tbdef1.WordWrap = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(218, 134);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(84, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "macro text prefix";
            // 
            // tbmen1
            // 
            this.tbmen1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbmen1.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbmen1.Location = new System.Drawing.Point(308, 216);
            this.tbmen1.Name = "tbmen1";
            this.tbmen1.Size = new System.Drawing.Size(778, 22);
            this.tbmen1.TabIndex = 8;
            this.tbmen1.TabStop = false;
            this.tbmen1.Text = "/* \\#*/\\t{\\m,\\t \\A,\\t \\P,\\t\\j,\\t\\p,\\tFLASH},";
            this.tbmen1.WordWrap = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(315, 15);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(90, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "menue item name";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // buttonGenerate
            // 
            this.buttonGenerate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonGenerate.Location = new System.Drawing.Point(984, 12);
            this.buttonGenerate.Name = "buttonGenerate";
            this.buttonGenerate.Size = new System.Drawing.Size(140, 62);
            this.buttonGenerate.TabIndex = 7;
            this.buttonGenerate.Text = "Generate";
            this.buttonGenerate.UseVisualStyleBackColor = true;
            this.buttonGenerate.Click += new System.EventHandler(this.buttonGenerate_Click_1);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(305, 43);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(100, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Action or Parameter";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // buttonLoad
            // 
            this.buttonLoad.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonLoad.Location = new System.Drawing.Point(1130, 52);
            this.buttonLoad.Name = "buttonLoad";
            this.buttonLoad.Size = new System.Drawing.Size(84, 34);
            this.buttonLoad.TabIndex = 6;
            this.buttonLoad.Text = "load";
            this.buttonLoad.UseVisualStyleBackColor = true;
            this.buttonLoad.Click += new System.EventHandler(this.buttonLoad_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(336, 69);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(69, 13);
            this.label7.TabIndex = 9;
            this.label7.Text = "Type of entry";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // buttonSave
            // 
            this.buttonSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSave.Location = new System.Drawing.Point(1130, 12);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(84, 34);
            this.buttonSave.TabIndex = 6;
            this.buttonSave.Text = "save";
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(333, 96);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(36, 13);
            this.label8.TabIndex = 9;
            this.label8.Text = "Upper";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxParValDefault
            // 
            this.textBoxParValDefault.Enabled = false;
            this.textBoxParValDefault.Location = new System.Drawing.Point(411, 119);
            this.textBoxParValDefault.Name = "textBoxParValDefault";
            this.textBoxParValDefault.Size = new System.Drawing.Size(55, 20);
            this.textBoxParValDefault.TabIndex = 5;
            this.textBoxParValDefault.TextChanged += new System.EventHandler(this.textBoxInfo_TextChanged);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(488, 96);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(36, 13);
            this.label10.TabIndex = 9;
            this.label10.Text = "Lower";
            this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxParValLower
            // 
            this.textBoxParValLower.Enabled = false;
            this.textBoxParValLower.Location = new System.Drawing.Point(530, 93);
            this.textBoxParValLower.Name = "textBoxParValLower";
            this.textBoxParValLower.Size = new System.Drawing.Size(55, 20);
            this.textBoxParValLower.TabIndex = 5;
            this.textBoxParValLower.TextChanged += new System.EventHandler(this.textBoxInfo_TextChanged);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(477, 122);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(47, 13);
            this.label11.TabIndex = 9;
            this.label11.Text = "ParType";
            this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxParValUpper
            // 
            this.textBoxParValUpper.Enabled = false;
            this.textBoxParValUpper.Location = new System.Drawing.Point(411, 93);
            this.textBoxParValUpper.Name = "textBoxParValUpper";
            this.textBoxParValUpper.Size = new System.Drawing.Size(55, 20);
            this.textBoxParValUpper.TabIndex = 5;
            this.textBoxParValUpper.TextChanged += new System.EventHandler(this.textBoxInfo_TextChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(364, 122);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(41, 13);
            this.label9.TabIndex = 9;
            this.label9.Text = "Default";
            this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxInfo
            // 
            this.textBoxInfo.Enabled = false;
            this.textBoxInfo.Location = new System.Drawing.Point(411, 40);
            this.textBoxInfo.Name = "textBoxInfo";
            this.textBoxInfo.Size = new System.Drawing.Size(174, 20);
            this.textBoxInfo.TabIndex = 5;
            this.textBoxInfo.TextChanged += new System.EventHandler(this.textBoxInfo_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(218, 163);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "text string";
            // 
            // textBoxNodeName
            // 
            this.textBoxNodeName.Location = new System.Drawing.Point(411, 14);
            this.textBoxNodeName.Name = "textBoxNodeName";
            this.textBoxNodeName.Size = new System.Drawing.Size(174, 20);
            this.textBoxNodeName.TabIndex = 5;
            this.textBoxNodeName.Text = "enter node name here";
            this.textBoxNodeName.TextChanged += new System.EventHandler(this.textBoxNodeName_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(218, 219);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "menue element";
            // 
            // textBoxResult
            // 
            this.textBoxResult.AcceptsReturn = true;
            this.textBoxResult.AcceptsTab = true;
            this.textBoxResult.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxResult.Location = new System.Drawing.Point(215, 244);
            this.textBoxResult.Multiline = true;
            this.textBoxResult.Name = "textBoxResult";
            this.textBoxResult.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxResult.Size = new System.Drawing.Size(999, 449);
            this.textBoxResult.TabIndex = 4;
            this.textBoxResult.TabStop = false;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(218, 192);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(37, 13);
            this.label12.TabIndex = 9;
            this.label12.Text = "create";
            // 
            // buttonRename
            // 
            this.buttonRename.Location = new System.Drawing.Point(215, 51);
            this.buttonRename.Name = "buttonRename";
            this.buttonRename.Size = new System.Drawing.Size(84, 34);
            this.buttonRename.TabIndex = 3;
            this.buttonRename.Text = "change";
            this.buttonRename.UseVisualStyleBackColor = true;
            this.buttonRename.Click += new System.EventHandler(this.buttonChange_Click);
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.Items.AddRange(new object[] {
            "normal",
            "back",
            "main menu",
            "parameter",
            "action"});
            this.comboBox1.Location = new System.Drawing.Point(411, 66);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(174, 21);
            this.comboBox1.TabIndex = 11;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // buttonDelete
            // 
            this.buttonDelete.Location = new System.Drawing.Point(215, 91);
            this.buttonDelete.Name = "buttonDelete";
            this.buttonDelete.Size = new System.Drawing.Size(84, 34);
            this.buttonDelete.TabIndex = 2;
            this.buttonDelete.Text = "delete";
            this.buttonDelete.UseVisualStyleBackColor = true;
            this.buttonDelete.Click += new System.EventHandler(this.buttonRemove_Click);
            // 
            // comboBoxParType
            // 
            this.comboBoxParType.Enabled = false;
            this.comboBoxParType.Location = new System.Drawing.Point(530, 119);
            this.comboBoxParType.Name = "comboBoxParType";
            this.comboBoxParType.Size = new System.Drawing.Size(70, 21);
            this.comboBoxParType.Sorted = true;
            this.comboBoxParType.TabIndex = 11;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(626, 23);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(168, 117);
            this.label6.TabIndex = 10;
            this.label6.Text = "\\t tab\r\n\\n newline\r\n\\# ID of element\r\n\\s String name of element\r\n\\m macrodisized " +
                "name of element \r\n\\p parent ID\r\n\\j jump target ID\r\n\\A action function\r\n\\P Parame" +
                "ter name";
            // 
            // checkBoxCreateEnum
            // 
            this.checkBoxCreateEnum.AutoSize = true;
            this.checkBoxCreateEnum.Checked = true;
            this.checkBoxCreateEnum.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxCreateEnum.Location = new System.Drawing.Point(308, 188);
            this.checkBoxCreateEnum.Name = "checkBoxCreateEnum";
            this.checkBoxCreateEnum.Size = new System.Drawing.Size(126, 17);
            this.checkBoxCreateEnum.TabIndex = 12;
            this.checkBoxCreateEnum.Text = "Parameter type enum\r\n";
            this.checkBoxCreateEnum.UseVisualStyleBackColor = true;
            // 
            // buttonAdd
            // 
            this.buttonAdd.Location = new System.Drawing.Point(215, 12);
            this.buttonAdd.Name = "buttonAdd";
            this.buttonAdd.Size = new System.Drawing.Size(84, 34);
            this.buttonAdd.TabIndex = 1;
            this.buttonAdd.Text = "add";
            this.buttonAdd.UseVisualStyleBackColor = true;
            this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Checked = true;
            this.checkBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox1.Location = new System.Drawing.Point(440, 188);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(117, 17);
            this.checkBox1.TabIndex = 12;
            this.checkBox1.Text = "Parameter Initializer\r\n";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // treeView1
            // 
            this.treeView1.AllowDrop = true;
            this.treeView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.treeView1.HideSelection = false;
            this.treeView1.ImageIndex = 0;
            this.treeView1.ImageList = this.imageList1;
            this.treeView1.LabelEdit = true;
            this.treeView1.Location = new System.Drawing.Point(12, 12);
            this.treeView1.Name = "treeView1";
            treeNode1.Name = "root";
            treeNode1.Text = "root";
            this.treeView1.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1});
            this.treeView1.SelectedImageIndex = 0;
            this.treeView1.Size = new System.Drawing.Size(193, 681);
            this.treeView1.TabIndex = 0;
            this.treeView1.DragDrop += new System.Windows.Forms.DragEventHandler(this.treeView1_DragDrop);
            this.treeView1.DragEnter += new System.Windows.Forms.DragEventHandler(this.treeView1_DragEnter);
            this.treeView1.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeView1_NodeMouseClick);
            this.treeView1.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.treeView1_ItemDrag);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1226, 705);
            this.Controls.Add(this.treeView1);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.buttonAdd);
            this.Controls.Add(this.checkBoxCreateEnum);
            this.Controls.Add(this.textBoxtxtPrefix);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.tbdef1);
            this.Controls.Add(this.comboBoxParType);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.buttonDelete);
            this.Controls.Add(this.tbmen1);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.buttonRename);
            this.Controls.Add(this.buttonGenerate);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textBoxResult);
            this.Controls.Add(this.buttonLoad);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.textBoxNodeName);
            this.Controls.Add(this.buttonSave);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.textBoxInfo);
            this.Controls.Add(this.textBoxParValDefault);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.textBoxParValUpper);
            this.Controls.Add(this.textBoxParValLower);
            this.Controls.Add(this.label11);
            this.MinimumSize = new System.Drawing.Size(1000, 600);
            this.Name = "Form1";
            this.Text = "TreeNodeBuilder for Jörgs Menu";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.TextBox textBoxtxtPrefix;
        private System.Windows.Forms.TextBox tbdef1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbmen1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonGenerate;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button buttonLoad;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button buttonSave;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBoxParValDefault;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBoxParValLower;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox textBoxParValUpper;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textBoxInfo;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxNodeName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxResult;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button buttonRename;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Button buttonDelete;
        private System.Windows.Forms.ComboBox comboBoxParType;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.CheckBox checkBoxCreateEnum;
        private System.Windows.Forms.Button buttonAdd;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.TreeView treeView1;
    }
}

