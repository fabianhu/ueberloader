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
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("root");
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.buttonAdd = new System.Windows.Forms.Button();
            this.buttonDelete = new System.Windows.Forms.Button();
            this.buttonRename = new System.Windows.Forms.Button();
            this.textBoxResult = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.buttonSave = new System.Windows.Forms.Button();
            this.buttonLoad = new System.Windows.Forms.Button();
            this.buttonGenerate = new System.Windows.Forms.Button();
            this.tbmen1 = new System.Windows.Forms.TextBox();
            this.tbdef1 = new System.Windows.Forms.TextBox();
            this.textBoxtxtPrefix = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // treeView1
            // 
            this.treeView1.HideSelection = false;
            this.treeView1.Location = new System.Drawing.Point(12, 62);
            this.treeView1.Name = "treeView1";
            treeNode2.Name = "root";
            treeNode2.Text = "root";
            this.treeView1.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode2});
            this.treeView1.Size = new System.Drawing.Size(290, 660);
            this.treeView1.TabIndex = 0;
            // 
            // buttonAdd
            // 
            this.buttonAdd.Location = new System.Drawing.Point(308, 36);
            this.buttonAdd.Name = "buttonAdd";
            this.buttonAdd.Size = new System.Drawing.Size(84, 34);
            this.buttonAdd.TabIndex = 1;
            this.buttonAdd.Text = "add here";
            this.buttonAdd.UseVisualStyleBackColor = true;
            this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
            // 
            // buttonDelete
            // 
            this.buttonDelete.Location = new System.Drawing.Point(308, 76);
            this.buttonDelete.Name = "buttonDelete";
            this.buttonDelete.Size = new System.Drawing.Size(84, 34);
            this.buttonDelete.TabIndex = 2;
            this.buttonDelete.Text = "delete";
            this.buttonDelete.UseVisualStyleBackColor = true;
            this.buttonDelete.Click += new System.EventHandler(this.buttonRemove_Click);
            // 
            // buttonRename
            // 
            this.buttonRename.Location = new System.Drawing.Point(308, 116);
            this.buttonRename.Name = "buttonRename";
            this.buttonRename.Size = new System.Drawing.Size(84, 34);
            this.buttonRename.TabIndex = 3;
            this.buttonRename.Text = "rename";
            this.buttonRename.UseVisualStyleBackColor = true;
            this.buttonRename.Click += new System.EventHandler(this.buttonRename_Click);
            // 
            // textBoxResult
            // 
            this.textBoxResult.AcceptsReturn = true;
            this.textBoxResult.AcceptsTab = true;
            this.textBoxResult.Location = new System.Drawing.Point(308, 322);
            this.textBoxResult.Multiline = true;
            this.textBoxResult.Name = "textBoxResult";
            this.textBoxResult.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxResult.Size = new System.Drawing.Size(824, 371);
            this.textBoxResult.TabIndex = 4;
            this.textBoxResult.TabStop = false;
            // 
            // textBox2
            // 
            this.textBox2.CharacterCasing = System.Windows.Forms.CharacterCasing.Lower;
            this.textBox2.Location = new System.Drawing.Point(12, 36);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(290, 20);
            this.textBox2.TabIndex = 5;
            this.textBox2.Text = "enter node name here";
            this.textBox2.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.textBox2_PreviewKeyDown);
            this.textBox2.Enter += new System.EventHandler(this.textBox2_Enter);
            // 
            // buttonSave
            // 
            this.buttonSave.Location = new System.Drawing.Point(308, 156);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(84, 34);
            this.buttonSave.TabIndex = 6;
            this.buttonSave.Text = "save";
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // buttonLoad
            // 
            this.buttonLoad.Location = new System.Drawing.Point(308, 196);
            this.buttonLoad.Name = "buttonLoad";
            this.buttonLoad.Size = new System.Drawing.Size(84, 34);
            this.buttonLoad.TabIndex = 6;
            this.buttonLoad.Text = "load";
            this.buttonLoad.UseVisualStyleBackColor = true;
            this.buttonLoad.Click += new System.EventHandler(this.buttonLoad_Click);
            // 
            // buttonGenerate
            // 
            this.buttonGenerate.Location = new System.Drawing.Point(800, 36);
            this.buttonGenerate.Name = "buttonGenerate";
            this.buttonGenerate.Size = new System.Drawing.Size(332, 83);
            this.buttonGenerate.TabIndex = 7;
            this.buttonGenerate.Text = "Generate";
            this.buttonGenerate.UseVisualStyleBackColor = true;
            this.buttonGenerate.Click += new System.EventHandler(this.buttonGenerate_Click_1);
            // 
            // tbmen1
            // 
            this.tbmen1.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbmen1.Location = new System.Drawing.Point(398, 291);
            this.tbmen1.Name = "tbmen1";
            this.tbmen1.Size = new System.Drawing.Size(717, 22);
            this.tbmen1.TabIndex = 8;
            this.tbmen1.TabStop = false;
            this.tbmen1.Text = "/* \\#*/\\t{\\m,\\t 0,\\t 0,\\t\\j,\\t\\p,\\tFLASH},";
            this.tbmen1.WordWrap = false;
            // 
            // tbdef1
            // 
            this.tbdef1.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbdef1.Location = new System.Drawing.Point(398, 265);
            this.tbdef1.Name = "tbdef1";
            this.tbdef1.Size = new System.Drawing.Size(717, 22);
            this.tbdef1.TabIndex = 8;
            this.tbdef1.TabStop = false;
            this.tbdef1.Text = "char\\t\\m[] \\tPROGMEM=\"\\s\";";
            this.tbdef1.WordWrap = false;
            // 
            // textBoxtxtPrefix
            // 
            this.textBoxtxtPrefix.AcceptsTab = true;
            this.textBoxtxtPrefix.Location = new System.Drawing.Point(398, 236);
            this.textBoxtxtPrefix.Name = "textBoxtxtPrefix";
            this.textBoxtxtPrefix.Size = new System.Drawing.Size(29, 20);
            this.textBoxtxtPrefix.TabIndex = 8;
            this.textBoxtxtPrefix.TabStop = false;
            this.textBoxtxtPrefix.Text = "txt";
            this.textBoxtxtPrefix.WordWrap = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(308, 239);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(84, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "macro text prefix";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(797, 171);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(168, 91);
            this.label6.TabIndex = 10;
            this.label6.Text = "\\t tab\r\n\\n newline\r\n\\# ID of element\r\n\\s String name of element\r\n\\m macrodisized " +
                "name of element \r\n\\p parent ID\r\n\\j jump target ID";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(308, 268);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "text string";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(308, 294);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "menue element";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1201, 705);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textBoxtxtPrefix);
            this.Controls.Add(this.tbdef1);
            this.Controls.Add(this.tbmen1);
            this.Controls.Add(this.buttonGenerate);
            this.Controls.Add(this.buttonLoad);
            this.Controls.Add(this.buttonSave);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBoxResult);
            this.Controls.Add(this.buttonRename);
            this.Controls.Add(this.buttonDelete);
            this.Controls.Add(this.buttonAdd);
            this.Controls.Add(this.treeView1);
            this.Name = "Form1";
            this.Text = "TreeNodeBuilder for Jörgs Menu";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.Button buttonAdd;
        private System.Windows.Forms.Button buttonDelete;
        private System.Windows.Forms.Button buttonRename;
        private System.Windows.Forms.TextBox textBoxResult;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Button buttonSave;
        private System.Windows.Forms.Button buttonLoad;
        private System.Windows.Forms.Button buttonGenerate;
        private System.Windows.Forms.TextBox tbmen1;
        private System.Windows.Forms.TextBox tbdef1;
        private System.Windows.Forms.TextBox textBoxtxtPrefix;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
    }
}

