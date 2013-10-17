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
            this.label5 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonGenerate = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.buttonLoad = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.buttonSave = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.textBoxParValLower = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.textBoxParValUpper = new System.Windows.Forms.TextBox();
            this.textBoxInfo = new System.Windows.Forms.TextBox();
            this.textBoxNodeName = new System.Windows.Forms.TextBox();
            this.textBoxResult = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.buttonRename = new System.Windows.Forms.Button();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.buttonDelete = new System.Windows.Forms.Button();
            this.comboBoxParType = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.checkBoxCreateEnum = new System.Windows.Forms.CheckBox();
            this.buttonAdd = new System.Windows.Forms.Button();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.label13 = new System.Windows.Forms.Label();
            this.textBoxParStepSize = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.textBoxParStructName = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.splitContainer3 = new System.Windows.Forms.SplitContainer();
            this.textBoxResult2 = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).BeginInit();
            this.splitContainer3.Panel1.SuspendLayout();
            this.splitContainer3.Panel2.SuspendLayout();
            this.splitContainer3.SuspendLayout();
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
            this.textBoxtxtPrefix.Location = new System.Drawing.Point(93, 164);
            this.textBoxtxtPrefix.Name = "textBoxtxtPrefix";
            this.textBoxtxtPrefix.Size = new System.Drawing.Size(38, 20);
            this.textBoxtxtPrefix.TabIndex = 8;
            this.textBoxtxtPrefix.TabStop = false;
            this.textBoxtxtPrefix.Text = "txt";
            this.textBoxtxtPrefix.Visible = false;
            this.textBoxtxtPrefix.WordWrap = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 168);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(84, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "macro text prefix";
            this.label5.Visible = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(123, 6);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(90, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "menue item name";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // buttonGenerate
            // 
            this.buttonGenerate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonGenerate.Location = new System.Drawing.Point(732, 47);
            this.buttonGenerate.Name = "buttonGenerate";
            this.buttonGenerate.Size = new System.Drawing.Size(93, 38);
            this.buttonGenerate.TabIndex = 7;
            this.buttonGenerate.Text = "Generate";
            this.buttonGenerate.UseVisualStyleBackColor = true;
            this.buttonGenerate.Click += new System.EventHandler(this.buttonGenerate_Click_1);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(113, 32);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(100, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Action or Parameter";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // buttonLoad
            // 
            this.buttonLoad.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonLoad.Location = new System.Drawing.Point(732, 3);
            this.buttonLoad.Name = "buttonLoad";
            this.buttonLoad.Size = new System.Drawing.Size(93, 38);
            this.buttonLoad.TabIndex = 6;
            this.buttonLoad.Text = "load";
            this.buttonLoad.UseVisualStyleBackColor = true;
            this.buttonLoad.Click += new System.EventHandler(this.buttonLoad_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(144, 58);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(69, 13);
            this.label7.TabIndex = 9;
            this.label7.Text = "Type of entry";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // buttonSave
            // 
            this.buttonSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSave.Location = new System.Drawing.Point(732, 91);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(93, 38);
            this.buttonSave.TabIndex = 6;
            this.buttonSave.Text = "save";
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(177, 85);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(36, 13);
            this.label8.TabIndex = 9;
            this.label8.Text = "Upper";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(177, 111);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(36, 13);
            this.label10.TabIndex = 9;
            this.label10.Text = "Lower";
            this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxParValLower
            // 
            this.textBoxParValLower.Enabled = false;
            this.textBoxParValLower.Location = new System.Drawing.Point(220, 108);
            this.textBoxParValLower.Name = "textBoxParValLower";
            this.textBoxParValLower.Size = new System.Drawing.Size(64, 20);
            this.textBoxParValLower.TabIndex = 5;
            this.textBoxParValLower.TextChanged += new System.EventHandler(this.textBoxInfo_TextChanged);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(295, 111);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(47, 13);
            this.label11.TabIndex = 9;
            this.label11.Text = "ParType";
            this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxParValUpper
            // 
            this.textBoxParValUpper.Enabled = false;
            this.textBoxParValUpper.Location = new System.Drawing.Point(220, 82);
            this.textBoxParValUpper.Name = "textBoxParValUpper";
            this.textBoxParValUpper.Size = new System.Drawing.Size(64, 20);
            this.textBoxParValUpper.TabIndex = 5;
            this.textBoxParValUpper.TextChanged += new System.EventHandler(this.textBoxInfo_TextChanged);
            // 
            // textBoxInfo
            // 
            this.textBoxInfo.Enabled = false;
            this.textBoxInfo.Location = new System.Drawing.Point(220, 29);
            this.textBoxInfo.Name = "textBoxInfo";
            this.textBoxInfo.Size = new System.Drawing.Size(183, 20);
            this.textBoxInfo.TabIndex = 5;
            this.textBoxInfo.TextChanged += new System.EventHandler(this.textBoxInfo_TextChanged);
            // 
            // textBoxNodeName
            // 
            this.textBoxNodeName.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxNodeName.Location = new System.Drawing.Point(220, 3);
            this.textBoxNodeName.Name = "textBoxNodeName";
            this.textBoxNodeName.Size = new System.Drawing.Size(183, 21);
            this.textBoxNodeName.TabIndex = 5;
            this.textBoxNodeName.Text = "enter node name here";
            this.textBoxNodeName.TextChanged += new System.EventHandler(this.textBoxNodeName_TextChanged);
            // 
            // textBoxResult
            // 
            this.textBoxResult.AcceptsReturn = true;
            this.textBoxResult.AcceptsTab = true;
            this.textBoxResult.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxResult.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxResult.Location = new System.Drawing.Point(0, 0);
            this.textBoxResult.Multiline = true;
            this.textBoxResult.Name = "textBoxResult";
            this.textBoxResult.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxResult.Size = new System.Drawing.Size(476, 481);
            this.textBoxResult.TabIndex = 4;
            this.textBoxResult.TabStop = false;
            this.textBoxResult.Click += new System.EventHandler(this.textBoxResult_Click);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(3, 191);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(37, 13);
            this.label12.TabIndex = 9;
            this.label12.Text = "create";
            this.label12.Visible = false;
            // 
            // buttonRename
            // 
            this.buttonRename.Location = new System.Drawing.Point(3, 47);
            this.buttonRename.Name = "buttonRename";
            this.buttonRename.Size = new System.Drawing.Size(93, 38);
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
            this.comboBox1.Location = new System.Drawing.Point(220, 55);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(183, 21);
            this.comboBox1.TabIndex = 11;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // buttonDelete
            // 
            this.buttonDelete.Location = new System.Drawing.Point(3, 91);
            this.buttonDelete.Name = "buttonDelete";
            this.buttonDelete.Size = new System.Drawing.Size(93, 38);
            this.buttonDelete.TabIndex = 2;
            this.buttonDelete.Text = "delete";
            this.buttonDelete.UseVisualStyleBackColor = true;
            this.buttonDelete.Click += new System.EventHandler(this.buttonRemove_Click);
            // 
            // comboBoxParType
            // 
            this.comboBoxParType.Enabled = false;
            this.comboBoxParType.Location = new System.Drawing.Point(348, 108);
            this.comboBoxParType.Name = "comboBoxParType";
            this.comboBoxParType.Size = new System.Drawing.Size(79, 21);
            this.comboBoxParType.Sorted = true;
            this.comboBoxParType.TabIndex = 11;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(459, 12);
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
            this.checkBoxCreateEnum.Location = new System.Drawing.Point(46, 190);
            this.checkBoxCreateEnum.Name = "checkBoxCreateEnum";
            this.checkBoxCreateEnum.Size = new System.Drawing.Size(126, 17);
            this.checkBoxCreateEnum.TabIndex = 12;
            this.checkBoxCreateEnum.Text = "Parameter type enum\r\n";
            this.checkBoxCreateEnum.UseVisualStyleBackColor = true;
            this.checkBoxCreateEnum.Visible = false;
            // 
            // buttonAdd
            // 
            this.buttonAdd.Location = new System.Drawing.Point(3, 3);
            this.buttonAdd.Name = "buttonAdd";
            this.buttonAdd.Size = new System.Drawing.Size(93, 38);
            this.buttonAdd.TabIndex = 1;
            this.buttonAdd.Text = "add";
            this.buttonAdd.UseVisualStyleBackColor = true;
            this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
            // 
            // treeView1
            // 
            this.treeView1.AllowDrop = true;
            this.treeView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeView1.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.treeView1.HideSelection = false;
            this.treeView1.ImageIndex = 0;
            this.treeView1.ImageList = this.imageList1;
            this.treeView1.LabelEdit = true;
            this.treeView1.Location = new System.Drawing.Point(0, 0);
            this.treeView1.Name = "treeView1";
            treeNode1.Name = "root";
            treeNode1.Text = "root";
            this.treeView1.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1});
            this.treeView1.SelectedImageIndex = 0;
            this.treeView1.Size = new System.Drawing.Size(256, 705);
            this.treeView1.TabIndex = 0;
            this.treeView1.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.treeView1_ItemDrag);
            this.treeView1.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeView1_NodeMouseClick);
            this.treeView1.DragDrop += new System.Windows.Forms.DragEventHandler(this.treeView1_DragDrop);
            this.treeView1.DragEnter += new System.Windows.Forms.DragEventHandler(this.treeView1_DragEnter);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(164, 137);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(49, 13);
            this.label13.TabIndex = 9;
            this.label13.Text = "StepSize";
            this.label13.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxParStepSize
            // 
            this.textBoxParStepSize.Enabled = false;
            this.textBoxParStepSize.Location = new System.Drawing.Point(219, 134);
            this.textBoxParStepSize.Name = "textBoxParStepSize";
            this.textBoxParStepSize.Size = new System.Drawing.Size(64, 20);
            this.textBoxParStepSize.TabIndex = 5;
            this.textBoxParStepSize.TextChanged += new System.EventHandler(this.textBoxInfo_TextChanged);
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(144, 167);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(70, 13);
            this.label14.TabIndex = 9;
            this.label14.Text = "par text prefix";
            this.label14.Visible = false;
            // 
            // textBox1
            // 
            this.textBox1.AcceptsTab = true;
            this.textBox1.Location = new System.Drawing.Point(220, 164);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(38, 20);
            this.textBox1.TabIndex = 8;
            this.textBox1.TabStop = false;
            this.textBox1.Text = "par";
            this.textBox1.Visible = false;
            this.textBox1.WordWrap = false;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.IsSplitterFixed = true;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.treeView1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(1088, 705);
            this.splitContainer1.SplitterDistance = 256;
            this.splitContainer1.TabIndex = 13;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer2.IsSplitterFixed = true;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.textBoxParStructName);
            this.splitContainer2.Panel1.Controls.Add(this.buttonLoad);
            this.splitContainer2.Panel1.Controls.Add(this.comboBox1);
            this.splitContainer2.Panel1.Controls.Add(this.comboBoxParType);
            this.splitContainer2.Panel1.Controls.Add(this.buttonSave);
            this.splitContainer2.Panel1.Controls.Add(this.label3);
            this.splitContainer2.Panel1.Controls.Add(this.checkBoxCreateEnum);
            this.splitContainer2.Panel1.Controls.Add(this.label11);
            this.splitContainer2.Panel1.Controls.Add(this.buttonGenerate);
            this.splitContainer2.Panel1.Controls.Add(this.textBoxParValLower);
            this.splitContainer2.Panel1.Controls.Add(this.buttonAdd);
            this.splitContainer2.Panel1.Controls.Add(this.textBoxParValUpper);
            this.splitContainer2.Panel1.Controls.Add(this.label12);
            this.splitContainer2.Panel1.Controls.Add(this.label10);
            this.splitContainer2.Panel1.Controls.Add(this.buttonDelete);
            this.splitContainer2.Panel1.Controls.Add(this.label4);
            this.splitContainer2.Panel1.Controls.Add(this.buttonRename);
            this.splitContainer2.Panel1.Controls.Add(this.label13);
            this.splitContainer2.Panel1.Controls.Add(this.textBoxParStepSize);
            this.splitContainer2.Panel1.Controls.Add(this.textBoxInfo);
            this.splitContainer2.Panel1.Controls.Add(this.label5);
            this.splitContainer2.Panel1.Controls.Add(this.label8);
            this.splitContainer2.Panel1.Controls.Add(this.textBox1);
            this.splitContainer2.Panel1.Controls.Add(this.label7);
            this.splitContainer2.Panel1.Controls.Add(this.label9);
            this.splitContainer2.Panel1.Controls.Add(this.label14);
            this.splitContainer2.Panel1.Controls.Add(this.textBoxNodeName);
            this.splitContainer2.Panel1.Controls.Add(this.textBoxtxtPrefix);
            this.splitContainer2.Panel1.Controls.Add(this.label6);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.splitContainer3);
            this.splitContainer2.Size = new System.Drawing.Size(828, 705);
            this.splitContainer2.SplitterDistance = 220;
            this.splitContainer2.TabIndex = 13;
            // 
            // textBoxParStructName
            // 
            this.textBoxParStructName.Location = new System.Drawing.Point(380, 165);
            this.textBoxParStructName.Name = "textBoxParStructName";
            this.textBoxParStructName.Size = new System.Drawing.Size(100, 20);
            this.textBoxParStructName.TabIndex = 13;
            this.textBoxParStructName.Text = "myPar";
            this.textBoxParStructName.Visible = false;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(294, 168);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(80, 13);
            this.label9.TabIndex = 9;
            this.label9.Text = "par struct name";
            this.label9.Visible = false;
            // 
            // splitContainer3
            // 
            this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer3.Location = new System.Drawing.Point(0, 0);
            this.splitContainer3.Name = "splitContainer3";
            // 
            // splitContainer3.Panel1
            // 
            this.splitContainer3.Panel1.Controls.Add(this.textBoxResult);
            // 
            // splitContainer3.Panel2
            // 
            this.splitContainer3.Panel2.Controls.Add(this.textBoxResult2);
            this.splitContainer3.Size = new System.Drawing.Size(828, 481);
            this.splitContainer3.SplitterDistance = 476;
            this.splitContainer3.TabIndex = 0;
            // 
            // textBoxResult2
            // 
            this.textBoxResult2.AcceptsReturn = true;
            this.textBoxResult2.AcceptsTab = true;
            this.textBoxResult2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxResult2.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxResult2.Location = new System.Drawing.Point(0, 0);
            this.textBoxResult2.Multiline = true;
            this.textBoxResult2.Name = "textBoxResult2";
            this.textBoxResult2.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxResult2.Size = new System.Drawing.Size(348, 481);
            this.textBoxResult2.TabIndex = 4;
            this.textBoxResult2.TabStop = false;
            this.textBoxResult2.Click += new System.EventHandler(this.textBoxResult2_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1088, 705);
            this.Controls.Add(this.splitContainer1);
            this.MinimumSize = new System.Drawing.Size(1000, 600);
            this.Name = "Form1";
            this.Text = "TreeNodeBuilder for Jörgs Menu";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel1.PerformLayout();
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.splitContainer3.Panel1.ResumeLayout(false);
            this.splitContainer3.Panel1.PerformLayout();
            this.splitContainer3.Panel2.ResumeLayout(false);
            this.splitContainer3.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).EndInit();
            this.splitContainer3.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.TextBox textBoxtxtPrefix;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonGenerate;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button buttonLoad;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button buttonSave;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBoxParValLower;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox textBoxParValUpper;
        private System.Windows.Forms.TextBox textBoxInfo;
        private System.Windows.Forms.TextBox textBoxNodeName;
        private System.Windows.Forms.TextBox textBoxResult;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button buttonRename;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Button buttonDelete;
        private System.Windows.Forms.ComboBox comboBoxParType;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.CheckBox checkBoxCreateEnum;
        private System.Windows.Forms.Button buttonAdd;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox textBoxParStepSize;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TextBox textBoxResult2;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.SplitContainer splitContainer3;
        private System.Windows.Forms.TextBox textBoxParStructName;
        private System.Windows.Forms.Label label9;
    }
}

