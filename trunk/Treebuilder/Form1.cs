using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace Treebuilder
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            CheckTagPresenceOfNode(treeView1.Nodes[0]);
        }

        // Xml tag for node, e.g. 'node' in case of <node></node>

        private const string XmlNodeTag = "node";

        // Xml attributes for node e.g. <node text="Asia" tag="" 

        private const string XmlNodeTextAtt = "text";
        private const string XmlNodeTagTypeAtt = "tagType";
        private const string XmlNodeTagIDAtt = "tagID";
        private const string XmlNodeTagInfoAtt = "tagInfo";
        private const string XmlNodeImageIndexAtt = "imageindex";
        private const string XmlNodeTagParUpper = "tagParUpper";
        private const string XmlNodeTagParLower = "tagParLower";
        private const string XmlNodeTagParDefault = "tagParDefault";
        private const string XmlNodeTagParType = "tagParType";

        ArrayList alRememberNodeNames = new ArrayList();

        enum MenueElementType
        {
            normal = 0,  // do not change numbers, as the combo box relies on this
            back = 1,
            main = 2,
            parameter = 3,
            action = 4,
        }

        struct NodeTagInfo
        {
            public MenueElementType type;
            public int ID;
            public string info;
            public int ParUpper;
            public int ParLower;
            public int ParDefault;
            public string ParType;
        }



        public void DeserializeTreeView(TreeView treeView, string fileName)
        {
            XmlTextReader reader = null;
            try
            {
                // disabling re-drawing of treeview till all nodes are added

                treeView.BeginUpdate();
                reader = new XmlTextReader(fileName);
                TreeNode parentNode = null;
                while (reader.Read())
                {
                    if (reader.NodeType == XmlNodeType.Element)
                    {
                        if (reader.Name == XmlNodeTag)
                        {
                            TreeNode newNode = new TreeNode();
                            bool isEmptyElement = reader.IsEmptyElement;

                            // loading node attributes

                            int attributeCount = reader.AttributeCount;
                            if (attributeCount > 0)
                            {
                                for (int i = 0; i < attributeCount; i++)
                                {
                                    reader.MoveToAttribute(i);
                                    SetAttributeValue(newNode,
                                                 reader.Name, reader.Value);
                                }
                            }
                            // add new node to Parent Node or TreeView

                            if (parentNode != null)
                                parentNode.Nodes.Add(newNode);
                            else
                                treeView.Nodes.Add(newNode);

                            // making current node 'ParentNode' if its not empty

                            if (!isEmptyElement)
                            {
                                parentNode = newNode;
                            }
                        }
                    }
                    // moving up to in TreeView if end tag is encountered

                    else if (reader.NodeType == XmlNodeType.EndElement)
                    {
                        if (reader.Name == XmlNodeTag)
                        {
                            parentNode = parentNode.Parent;
                        }
                    }
                    else if (reader.NodeType == XmlNodeType.XmlDeclaration)
                    {
                        //Ignore Xml Declaration                    

                    }
                    else if (reader.NodeType == XmlNodeType.None)
                    {
                        return;
                    }
                    else if (reader.NodeType == XmlNodeType.Text)
                    {
                        parentNode.Nodes.Add(reader.Value);
                    }

                }
            }
            finally
            {
                // enabling redrawing of treeview after all nodes are added

                treeView.EndUpdate();
                reader.Close();
            }
        }

        /// <summary>

        /// Used by Deserialize method for setting properties of 

        /// TreeNode from xml node attributes

        /// </summary>

        private void SetAttributeValue(TreeNode node,
                           string propertyName, string value)
        {
            NodeTagInfo nti;

            if (node.Tag == null)
            {
                nti = new NodeTagInfo();
                nti.type = MenueElementType.normal;
                nti.info = "";
                nti.ID = -1;
                node.Tag = nti;
            }

            if (propertyName == XmlNodeTextAtt)
            {
                node.Text = value;
            }
            else if (propertyName == XmlNodeImageIndexAtt)
            {
                node.ImageIndex = int.Parse(value);
                node.SelectedImageIndex = int.Parse(value);
            }
            else if (propertyName == XmlNodeTagIDAtt)
            {
                nti = (NodeTagInfo)node.Tag;
                nti.ID = int.Parse(value);
                node.Tag = nti;
            }
            else if (propertyName == XmlNodeTagTypeAtt)
            {
                nti = (NodeTagInfo)node.Tag;
                EnumConverter ec = new EnumConverter(typeof(MenueElementType));
                nti.type = (MenueElementType)ec.ConvertFromString(value);
                node.Tag = nti;
            }
            else if (propertyName == XmlNodeTagInfoAtt)
            {
                nti = (NodeTagInfo)node.Tag;
                nti.info = value;
                node.Tag = nti;
            }
            else if (propertyName == XmlNodeTagParUpper)
            {
                nti = (NodeTagInfo)node.Tag;
                nti.ParUpper = int.Parse(value);
                node.Tag = nti;
            }
            else if (propertyName == XmlNodeTagParLower)
            {
                nti = (NodeTagInfo)node.Tag;
                nti.ParLower = int.Parse(value);
                node.Tag = nti;
            }
            else if (propertyName == XmlNodeTagParDefault)
            {
                nti = (NodeTagInfo)node.Tag;
                nti.ParDefault = int.Parse(value);
                node.Tag = nti;
            }
            else if (propertyName == XmlNodeTagParType)
            {
                nti = (NodeTagInfo)node.Tag;
                nti.ParType = value;
                node.Tag = nti;

                node.ImageIndex = (int)((NodeTagInfo)node.Tag).type; // this is duplicated somehow..., but if node info is missing in XML, it is restored here.
                node.SelectedImageIndex = (int)((NodeTagInfo)node.Tag).type;
            }
        }

        public void SerializeTreeView(TreeView treeView, string fileName)
        {
            XmlTextWriter textWriter = new XmlTextWriter(fileName,
                                          System.Text.Encoding.ASCII);
            // writing the xml declaration tag

            textWriter.WriteStartDocument();
            //textWriter.WriteRaw("\r\n");

            // writing the main tag that encloses all node tags

            textWriter.WriteStartElement("TreeView");

            // save the nodes, recursive method

            SaveNodes(treeView.Nodes, textWriter);

            textWriter.WriteEndElement();

            textWriter.Close();
        }

        private void SaveNodes(TreeNodeCollection nodesCollection, XmlTextWriter textWriter)
        {
            for (int i = 0; i < nodesCollection.Count; i++)
            {
                TreeNode node = nodesCollection[i];
                textWriter.WriteStartElement(XmlNodeTag);
                textWriter.WriteAttributeString(XmlNodeTextAtt,
                                                           node.Text);
                textWriter.WriteAttributeString(
                    XmlNodeImageIndexAtt, node.ImageIndex.ToString());
                if (node.Tag != null)
                {
                    textWriter.WriteAttributeString(XmlNodeTagTypeAtt, ((NodeTagInfo)node.Tag).type.ToString());
                    textWriter.WriteAttributeString(XmlNodeTagIDAtt, ((NodeTagInfo)node.Tag).ID.ToString());
                    if (((NodeTagInfo)node.Tag).info != null)
                        textWriter.WriteAttributeString(XmlNodeTagInfoAtt, ((NodeTagInfo)node.Tag).info.ToString());
                    else
                        textWriter.WriteAttributeString(XmlNodeTagInfoAtt, "");
                    textWriter.WriteAttributeString(XmlNodeTagParDefault, ((NodeTagInfo)node.Tag).ParDefault.ToString());
                    textWriter.WriteAttributeString(XmlNodeTagParUpper, ((NodeTagInfo)node.Tag).ParUpper.ToString());
                    textWriter.WriteAttributeString(XmlNodeTagParLower, ((NodeTagInfo)node.Tag).ParLower.ToString());
                    if (((NodeTagInfo)node.Tag).ParType != null)
                        textWriter.WriteAttributeString(XmlNodeTagParType, ((NodeTagInfo)node.Tag).ParType.ToString());
                    else
                        textWriter.WriteAttributeString(XmlNodeTagParType, "");
                }
                // add other node properties to serialize here  

                if (node.Nodes.Count > 0)
                {
                    SaveNodes(node.Nodes, textWriter);
                }
                textWriter.WriteEndElement();
            }
        }


        private void buttonSave_Click(object sender, EventArgs e)
        {
            // save
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.DefaultExt = "*.jmdef";
            sfd.Filter = "Jörg Menue Definition (*.jmdef)|*.jmdef";
            sfd.OverwritePrompt = true;
            sfd.ValidateNames = true;
            DialogResult res = sfd.ShowDialog();
            if (res == DialogResult.OK && sfd.FileName != "")
            {
                SerializeTreeView(treeView1, sfd.FileName);
            }
        }

        private void buttonLoad_Click(object sender, EventArgs e)
        {
            // load
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.DefaultExt = "*.jmdef";
            ofd.Filter = "Jörg Menue Definition (*.jmdef)|*.jmdef";
            ofd.ValidateNames = true;
            DialogResult res = ofd.ShowDialog();
            if (res == DialogResult.OK && ofd.FileName != "")
            {
                treeView1.Nodes.Clear();
                DeserializeTreeView(treeView1, ofd.FileName);
            }
            treeView1.ExpandAll();
        }



        private void textBoxNodeName_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyCode == Keys.Return && false) // fixme
            {
                buttonAdd_Click(this, EventArgs.Empty);
                textBoxNodeName.SelectAll();
            }
        }

        int number; // zählt die Zeile mit, um den parent index zu finden = ID in der Ausgabe.


        private void buttonGenerate_Click_1(object sender, EventArgs e)
        {
            alRememberNodeNames.Clear();

            textBoxResult.Clear();

            TreeNode tn = treeView1.Nodes[0];

            textBoxResult.AppendText("******** START OF AUTO-GENERATED CODE DO NOT EDIT!!! *********" + Environment.NewLine + Environment.NewLine);


            // some Defines
            textBoxResult.AppendText(" #define\tMENUESIZE\t" + treeView1.GetNodeCount(true) + "\t// number of menu itmes (array size)" + Environment.NewLine);
            textBoxResult.AppendText(" #define\tMAX_ITEM_NAME_CHARLENGTH\t" + GetSubNodeMaxNameLength(treeView1.Nodes[0]) + "\t// number of menu itmes (array size)" + Environment.NewLine);

    // Parameter enum
            if (checkBoxCreateEnum.Checked)
            {
                textBoxResult.AppendText("// Enum definitions" + Environment.NewLine);
                textBoxResult.AppendText(
                    "typedef enum" + Environment.NewLine +
                    "{" + Environment.NewLine);
                UpdateParComboBox(treeView1.Nodes[0]);
                foreach (string s in comboBoxParType.Items)
                {
                    textBoxResult.AppendText("\t" + s + "," + Environment.NewLine);
                }
                textBoxResult.AppendText(
                   "} eParameterType_t;" + Environment.NewLine);
            }
            
            textBoxResult.AppendText(Environment.NewLine);

            textBoxResult.AppendText("******** MOVE the upper lines to the menu_variant.h header file. *********" + Environment.NewLine + Environment.NewLine);
            
            // Process the text definitions
            textBoxResult.AppendText("// Text definitions" + Environment.NewLine);
            textBoxResult.AppendText("" + ProcessNode(tbdef1.Text, tn) + Environment.NewLine);
            number = 0; // fixme rename
            NodeTagInfo nt;
            nt = (NodeTagInfo)tn.Tag;
            nt.ID = number++;
            tn.Tag = nt;
            processTextDefinitions(tn);
            textBoxResult.AppendText(Environment.NewLine);

        
            // Parameter list
            textBoxResult.AppendText("// Parameter definitions" + Environment.NewLine);
            ProcessParameters(treeView1.Nodes[0]);
            textBoxResult.AppendText(Environment.NewLine);


            // Process the MENUE LIST
            textBoxResult.AppendText("\t\t\t//Name\tAct\tPar\tJmp\tParent\tMemory" + Environment.NewLine);
            textBoxResult.AppendText("MenuItem_t m_items[MENUESIZE] = { " + Environment.NewLine);
            //            textBoxResult.AppendText("MenuItem_t m_items[MENUESIZE] = {" + Environment.NewLine);	
            textBoxResult.AppendText("\t" + ProcessNode(tbmen1.Text, tn) + " " + Environment.NewLine);
            processMenuList(tn);
            textBoxResult.AppendText("}" + Environment.NewLine);

            textBoxResult.AppendText(Environment.NewLine +"******** END OF AUTO-GENERATED CODE DO NOT EDIT!!! *********" + Environment.NewLine);


            // copy to clipboard
            Clipboard.SetData(DataFormats.Text, textBoxResult.Text);
            //textBoxResult.Text.

        }

        void ProcessParameters(TreeNode tn)
        {
            if (((NodeTagInfo)tn.Tag).type == MenueElementType.parameter)
            {
                textBoxResult.AppendText("Parameter_t " + ((NodeTagInfo)tn.Tag).info + " = {");
                textBoxResult.AppendText("\t" + ((NodeTagInfo)tn.Tag).ParDefault + ", " + ((NodeTagInfo)tn.Tag).ParLower + ", " + ((NodeTagInfo)tn.Tag).ParUpper + ", " + ((NodeTagInfo)tn.Tag).ParType);
                textBoxResult.AppendText("}; " + Environment.NewLine);
            }
            foreach (TreeNode tn2 in tn.Nodes)
            {
                ProcessParameters(tn2);
            }
        }

        int GetSubNodeMaxNameLength(TreeNode tn)
        {
            int mitl = 0;
            mitl = Math.Max(tn.Text.Length, mitl);

            foreach (TreeNode tn2 in tn.Nodes)
            {
                mitl = Math.Max(GetSubNodeMaxNameLength(tn2), mitl);
            }

            return mitl;
        }

        void UpdateParComboBox(TreeNode tn)
        {
            foreach (TreeNode tn2 in tn.Nodes)
            {
                try
                {
                if (!comboBoxParType.Items.Contains(((NodeTagInfo)tn2.Tag).ParType) && ((NodeTagInfo)tn2.Tag).ParType != "")
                    comboBoxParType.Items.Add(((NodeTagInfo)tn2.Tag).ParType);

                }
                catch (Exception)
                {
                    
                    //throw;
                }

                UpdateParComboBox(tn2);
            }
        }


        string makename(TreeNode tn)
        {
            return (textBoxtxtPrefix.Text + CleanString(tn.Text));//+ tn.Level.ToString() + "_" + tn.Index.ToString());
        }



        private void processTextDefinitions(TreeNode ano)
        {

            // process all nodes here
            foreach (TreeNode tn in ano.Nodes)
            {
                if (alRememberNodeNames.Contains(tn.Text))
                {
                    // already present
                }
                else
                {
                    // new
                    alRememberNodeNames.Add(tn.Text);

                    NodeTagInfo tag;
                    tag = (NodeTagInfo)tn.Tag;
                    tag.ID = number++;
                    tn.Tag = tag;
                    textBoxResult.AppendText("" + ProcessNode(tbdef1.Text, tn) + " " + Environment.NewLine);
                }
            }

            // get all nodes nodes processed
            foreach (TreeNode tn in ano.Nodes)
            {
                processTextDefinitions(tn);
            }
        }



        private void processMenuList(TreeNode ano)
        {
            foreach (TreeNode tn in ano.Nodes)
            {
                textBoxResult.AppendText("\t" +ProcessNode(tbmen1.Text, tn) + " " + Environment.NewLine);
            }
            foreach (TreeNode tn in ano.Nodes)
            {
                processMenuList(tn);
            }
        }

        static public string CleanString(string s)
        {
            if (s != null && s.Length > 0)
            {
                StringBuilder sb = new StringBuilder(s.Length);
                foreach (char c in s)
                {
                    if (Char.IsLetter(c))
                        sb.Append(Char.ToUpper(c));
                }
                s = sb.ToString();
            }
            return s;
        }


        /*
            \t tab
            \n newline
            \# ID of element
            \s String name of element
            \m macrodisized name of element 
            \p parent ID
            \j jump target ID
         */

        private string ProcessNode(string s, TreeNode tn)
        {
            bool specialchar = false;

            if (s != null && s.Length > 0)
            {
                StringBuilder sb = new StringBuilder(s.Length);
                foreach (char c in s)
                {
                    if (c == '\\')
                    {
                        // remember, that nex one is special
                        specialchar = true;
                    }
                    else if (specialchar)
                    {
                        // this is the next one special char
                        switch (c)
                        {
                            case 't':
                                sb.Append('\t');
                                break;
                            case 'n':
                                sb.Append(Environment.NewLine);
                                break;
                            case '#':
                                sb.Append(((NodeTagInfo)tn.Tag).ID);
                                break;
                            case 's':
                                sb.Append(tn.Text);
                                break;
                            case 'm':
                                sb.Append(makename(tn));
                                break;
                            case 'p':
                                if (tn.Parent == null)
                                { sb.Append("0"); }
                                else
                                    sb.Append(((NodeTagInfo)tn.Parent.Tag).ID);
                                break;
                            case 'A':
                                if (((NodeTagInfo)tn.Tag).type == MenueElementType.action)
                                    sb.Append(((NodeTagInfo)tn.Tag).info);
                                else
                                    sb.Append("0");
                                break;
                            case 'P':
                                if (((NodeTagInfo)tn.Tag).type == MenueElementType.parameter)
                                    sb.Append("&" + ((NodeTagInfo)tn.Tag).info);
                                else
                                    sb.Append("0");
                                break;
                            case 'j':
                                NodeTagInfo nti;
                                if (tn.Nodes.Count > 0)
                                {
                                    // sub-nodes
                                    nti = (NodeTagInfo)tn.Nodes[0].Tag;
                                    sb.Append(nti.ID);
                                }
                                else if (((NodeTagInfo)tn.Tag).type == MenueElementType.main)
                                {
                                    // main menu (first one inside menu)
                                    sb.Append("1");
                                }
                                else if (((NodeTagInfo)tn.Tag).type == MenueElementType.back)
                                {
                                    // back: we need the first node in the nodes collection, where our parent is. 
                                    if (tn.Parent == null)
                                        sb.Append(" **ERROR_NO_PARENT** ");
                                    else
                                    {
                                        nti = (NodeTagInfo)tn.Parent.FirstNode.Tag;
                                        sb.Append(nti.ID);
                                    }
                                }
                                else
                                {
                                    // no action
                                    sb.Append("0");
                                }
                                break;
                            default:
                                sb.Append(" **ERROR** ");
                                break;
                        }
                        specialchar = false;
                    }
                    else
                    {
                        sb.Append(c);
                    }


                }
                s = sb.ToString();
            }
            return s;
        }

        private void treeView1_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            textBoxNodeName.Text = e.Node.Text;
            textBoxInfo.Text = ((NodeTagInfo)(e.Node.Tag)).info;
            comboBox1.SelectedIndex = (int)((NodeTagInfo)(e.Node.Tag)).type;
            comboBoxParType.Text = ((NodeTagInfo)(e.Node.Tag)).ParType;
            textBoxParValUpper.Text = ((NodeTagInfo)(e.Node.Tag)).ParUpper.ToString();
            textBoxParValLower.Text = ((NodeTagInfo)(e.Node.Tag)).ParLower.ToString();
            textBoxParValDefault.Text = ((NodeTagInfo)(e.Node.Tag)).ParDefault.ToString();
        }

        private void textBoxNodeName_TextChanged(object sender, EventArgs e)
        {
            /*if (treeView1.SelectedNode !=null)
            treeView1.SelectedNode.Text = textBoxNodeName.Text;*/
        }

        private void textBoxInfo_TextChanged(object sender, EventArgs e)
        {
            /*NodeTagInfo nti;

            if (treeView1.SelectedNode != null)
            {
               CheckTagPresenceOfSelectedNode();

                nti = (NodeTagInfo)(treeView1.SelectedNode.Tag);
                nti.info = textBoxInfo.Text;
                treeView1.SelectedNode.Tag = nti;
            }*/
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode != null)
            {

                TreeNode tn = treeView1.SelectedNode.Nodes.Add(textBoxNodeName.Text);
                treeView1.SelectedNode.Expand();

                UpdateNodeValuesFromFields(tn);

                textBoxNodeName.SelectAll();
            }
        }

        private void CheckTagPresenceOfNode(TreeNode tn)
        {
            NodeTagInfo nti;
            if (tn.Tag == null)
            {
                nti = new NodeTagInfo();
                nti.ID = -1;
                nti.info = "";
                nti.ParLower = 0;
                nti.ParUpper = 0;
                nti.ParType = "";
                nti.ParDefault = 0;
                nti.type = MenueElementType.normal;
                tn.Tag = nti;
            }
        }

        private void buttonRemove_Click(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode.Name != "root")
                treeView1.SelectedNode.Remove();
        }

        private void buttonChange_Click(object sender, EventArgs e)
        {
            //rename

            UpdateNodeValuesFromFields(treeView1.SelectedNode);
        }

        private void UpdateNodeValuesFromFields(TreeNode tn)
        {
            NodeTagInfo nti;

            if (tn != null)
            {
                CheckTagPresenceOfNode(tn);
                tn.Text = textBoxNodeName.Text;
                nti = (NodeTagInfo)(tn.Tag);
                nti.info = textBoxInfo.Text;

                try
                {
                    nti.ParUpper = Convert.ToInt32(textBoxParValUpper.Text);
                }
                catch (Exception)
                {
                    nti.ParUpper = 0;
                }
                try
                {
                    nti.ParLower = Convert.ToInt32(textBoxParValLower.Text);
                }
                catch (Exception)
                {
                    nti.ParLower = 0;
                }
                try
                {
                    nti.ParDefault = Convert.ToInt32(textBoxParValDefault.Text);
                }
                catch (Exception)
                {
                    nti.ParDefault = 0;
                }
                nti.ParType = comboBoxParType.Text;

                nti.type = (MenueElementType)comboBox1.SelectedIndex;
                tn.Tag = nti;

                tn.ImageIndex = (int)((NodeTagInfo)tn.Tag).type;
                tn.SelectedImageIndex = (int)((NodeTagInfo)tn.Tag).type;
            }

            comboBoxParType.Items.Clear();
            UpdateParComboBox(treeView1.Nodes[0]);
        }


        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            /*NodeTagInfo nti;

            //change value
            if (treeView1.SelectedNode != null)
            {
                CheckTagPresenceOfSelectedNode();
                nti = (NodeTagInfo)(treeView1.SelectedNode.Tag);
                
                nti.type = (MenueElementType)comboBox1.SelectedIndex;
                treeView1.SelectedNode.Tag = nti;
            }*/


            if (comboBox1.SelectedIndex == 3 || comboBox1.SelectedIndex == 4)
            {
                textBoxInfo.Enabled = true;
            }
            else
            {
                textBoxInfo.Enabled = false;
            }

            if (comboBox1.SelectedIndex == 3)
            {
                textBoxParValDefault.Enabled = true;
                textBoxParValLower.Enabled = true;
                textBoxParValUpper.Enabled = true;
                comboBoxParType.Enabled = true;
            }
            else
            {
                textBoxParValDefault.Enabled = false;
                textBoxParValLower.Enabled = false;
                textBoxParValUpper.Enabled = false;
                comboBoxParType.Enabled = false;
            }
        }

        TreeNode sourceNode;

        private void treeView1_ItemDrag(object sender, ItemDragEventArgs e)
        {
            sourceNode = (TreeNode)e.Item;
            DoDragDrop(e.Item, DragDropEffects.Move);
        }

        private void treeView1_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent("System.Windows.Forms.TreeNode", false))
                e.Effect = DragDropEffects.Move;
            else
                e.Effect = DragDropEffects.None;
        }

        private void treeView1_DragDrop(object sender, DragEventArgs e)
        {
            TreeNode NewNode;

            if (e.Data.GetDataPresent("System.Windows.Forms.TreeNode", false))
            {
                Point pt = ((TreeView)sender).PointToClient(new Point(e.X, e.Y));
                TreeNode DestinationNode = ((TreeView)sender).GetNodeAt(pt);
                NewNode = (TreeNode)e.Data.GetData("System.Windows.Forms.TreeNode");
                if (DestinationNode.TreeView == NewNode.TreeView)
                {
                    //DestinationNode.Nodes.Add((TreeNode)NewNode.Clone());
                    DestinationNode.Parent.Nodes.Insert(DestinationNode.Index + 1, (TreeNode)NewNode.Clone());
                    DestinationNode.ExpandAll();
                    //Remove Original Node
                    NewNode.Remove();
                }
            }

        }














    }
}

