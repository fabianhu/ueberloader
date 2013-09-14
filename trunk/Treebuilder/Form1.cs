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

        // Xml attributes for node e.g. 

        private const string XmlNodeTextAtt = "text";
        private const string XmlNodeTagTypeAtt = "tagType";
        private const string XmlNodeTagIDAtt = "tagID";
        private const string XmlNodeTagInfoAtt = "tagInfo";
        private const string XmlNodeImageIndexAtt = "imageindex";
        private const string XmlNodeTagParUpper = "tagParUpper";
        private const string XmlNodeTagParLower = "tagParLower";
        private const string XmlNodeTagParStepSize = "tagParStepSize";
        private const string XmlNodeTagParType = "tagParType";

        private string tbdef = "char\\t\\m[] \\tPROGMEM=\"\\s\";";
        private string tbmen = "/* \\#*/\\t{\\m,\\t \\A,\\t \\P,\\t\\j,\\t\\p,\\tFLASH},";

        ArrayList m_alRememberNodeNames = new ArrayList();


        int m_NodeCounterForID; // zählt die Zeile mit, um den parent index zu finden = ID in der Ausgabe.

        TreeNode m_DragDropSourceNode;

        enum eMenueElementType
        {
            normal = 0,  // do not change numbers, as the combo box relies on this
            back = 1,
            main = 2,
            parameter = 3,
            action = 4,
        }

        struct tNodeTagInfo
        {
            public eMenueElementType type;
            public int ID;
            public string info;
            public int ParUpper;
            public int ParLower;
            public int ParStepSize;
            public string ParType;
        }



        public void vDeserializeTreeView(TreeView _treeView, string _fileName)
        {
            XmlTextReader reader = null;
            try
            {
                // disabling re-drawing of treeview till all nodes are added

                _treeView.BeginUpdate();
                reader = new XmlTextReader(_fileName);
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
                                    vSetAttributeValue(newNode,
                                                 reader.Name, reader.Value);
                                }
                            }
                            // add new node to Parent Node or TreeView

                            if (parentNode != null)
                                parentNode.Nodes.Add(newNode);
                            else
                                _treeView.Nodes.Add(newNode);

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

                _treeView.EndUpdate();
                reader.Close();
            }
        }

        /// <summary>

        /// Used by Deserialize method for setting properties of 

        /// TreeNode from xml node attributes

        /// </summary>

        private void vSetAttributeValue(TreeNode _node,
                           string _propertyName, string _value)
        {
            tNodeTagInfo nti;

            if (_node.Tag == null)
            {
                nti = new tNodeTagInfo();
                nti.type = eMenueElementType.normal;
                nti.info = "";
                nti.ID = -1;
                _node.Tag = nti;
            }

            if (_propertyName == XmlNodeTextAtt)
            {
                _node.Text = _value;
            }
            else if (_propertyName == XmlNodeImageIndexAtt)
            {
                _node.ImageIndex = int.Parse(_value);
                _node.SelectedImageIndex = int.Parse(_value);
            }
            else if (_propertyName == XmlNodeTagIDAtt)
            {
                nti = (tNodeTagInfo)_node.Tag;
                nti.ID = -1;//int.Parse(value); // fixme
                _node.Tag = nti;
            }
            else if (_propertyName == XmlNodeTagTypeAtt)
            {
                nti = (tNodeTagInfo)_node.Tag;
                EnumConverter ec = new EnumConverter(typeof(eMenueElementType));
                nti.type = (eMenueElementType)ec.ConvertFromString(_value);
                _node.Tag = nti;
            }
            else if (_propertyName == XmlNodeTagInfoAtt)
            {
                nti = (tNodeTagInfo)_node.Tag;
                nti.info = _value;
                _node.Tag = nti;
            }
            else if (_propertyName == XmlNodeTagParUpper)
            {
                nti = (tNodeTagInfo)_node.Tag;
                nti.ParUpper = int.Parse(_value);
                _node.Tag = nti;
            }
            else if (_propertyName == XmlNodeTagParLower)
            {
                nti = (tNodeTagInfo)_node.Tag;
                nti.ParLower = int.Parse(_value);
                _node.Tag = nti;
            }
            else if (_propertyName == XmlNodeTagParStepSize)
            {
                nti = (tNodeTagInfo)_node.Tag;
                nti.ParStepSize = int.Parse(_value);
                _node.Tag = nti;
            }
            else if (_propertyName == XmlNodeTagParType)
            {
                nti = (tNodeTagInfo)_node.Tag;
                nti.ParType = _value;
                _node.Tag = nti;

                _node.ImageIndex = (int)((tNodeTagInfo)_node.Tag).type; // this is duplicated somehow..., but if node info is missing in XML, it is restored here.
                _node.SelectedImageIndex = (int)((tNodeTagInfo)_node.Tag).type;
            }
        }

        public void vSerializeTreeView(TreeView _treeView, string _fileName)
        {
            XmlTextWriter textWriter = new XmlTextWriter(_fileName,
                                          System.Text.Encoding.ASCII);

            textWriter.Formatting = Formatting.Indented;
            
            // writing the xml declaration tag

            textWriter.WriteStartDocument();
            //textWriter.WriteRaw("\r\n");

            // writing the main tag that encloses all node tags

            textWriter.WriteStartElement("TreeView");

            // save the nodes, recursive method

            vSaveNodesToXML(_treeView.Nodes, textWriter);

            textWriter.WriteEndElement();

            textWriter.Close();
        }

        private void vSaveNodesToXML(TreeNodeCollection _nodesCollection, XmlTextWriter _textWriter)
        {
            for (int i = 0; i < _nodesCollection.Count; i++)
            {
                TreeNode node = _nodesCollection[i];
                _textWriter.WriteStartElement(XmlNodeTag);
                _textWriter.WriteAttributeString(XmlNodeTextAtt,
                                                           node.Text);
                _textWriter.WriteAttributeString(
                    XmlNodeImageIndexAtt, node.ImageIndex.ToString());
                if (node.Tag != null)
                {
                    _textWriter.WriteAttributeString(XmlNodeTagTypeAtt, ((tNodeTagInfo)node.Tag).type.ToString());
                    _textWriter.WriteAttributeString(XmlNodeTagIDAtt, ((tNodeTagInfo)node.Tag).ID.ToString());
                    if (((tNodeTagInfo)node.Tag).info != null)
                        _textWriter.WriteAttributeString(XmlNodeTagInfoAtt, ((tNodeTagInfo)node.Tag).info.ToString());
                    else
                        _textWriter.WriteAttributeString(XmlNodeTagInfoAtt, "");
                    _textWriter.WriteAttributeString(XmlNodeTagParUpper, ((tNodeTagInfo)node.Tag).ParUpper.ToString());
                    _textWriter.WriteAttributeString(XmlNodeTagParLower, ((tNodeTagInfo)node.Tag).ParLower.ToString());
                    _textWriter.WriteAttributeString(XmlNodeTagParStepSize, ((tNodeTagInfo)node.Tag).ParStepSize.ToString());
                    if (((tNodeTagInfo)node.Tag).ParType != null)
                        _textWriter.WriteAttributeString(XmlNodeTagParType, ((tNodeTagInfo)node.Tag).ParType.ToString());
                    else
                        _textWriter.WriteAttributeString(XmlNodeTagParType, "");
                }
                // add other node properties to serialize here  

                if (node.Nodes.Count > 0)
                {
                    vSaveNodesToXML(node.Nodes, _textWriter);
                }
                _textWriter.WriteEndElement();
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
                vSerializeTreeView(treeView1, sfd.FileName);
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
                vDeserializeTreeView(treeView1, ofd.FileName);
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

        private void buttonGenerate_Click_1(object sender, EventArgs e)
        {
            m_alRememberNodeNames.Clear();

            textBoxResult.Clear();
            textBoxResult2.Clear();

            TreeNode tn = treeView1.Nodes[0];

            textBoxResult.AppendText("//******** START OF AUTO-GENERATED HEADER DO NOT EDIT!!! *********" + Environment.NewLine + Environment.NewLine);


            // some Defines
            textBoxResult.AppendText(" #define\tMENUESIZE\t" + treeView1.GetNodeCount(true) + "\t// number of menu itmes (array size)" + Environment.NewLine);
            textBoxResult.AppendText(" #define\tMAX_ITEM_NAME_CHARLENGTH\t" + GetSubNodeMaxNameLength(treeView1.Nodes[0]) + "\t// max name length" + Environment.NewLine);
        
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

            // Parameter externals
            textBoxResult.AppendText("// Parameter externals" + Environment.NewLine);
            textBoxResult.AppendText("typedef struct " + textBoxParStructName.Text+"_tag" + Environment.NewLine);
            textBoxResult.AppendText("{" + Environment.NewLine);

            parcount = 0; // will be counted by following fn, which is recursively called.
            ProcessParameterExternals(treeView1.Nodes[0]);
            textBoxResult.AppendText("\tuint8_t crc8;" + Environment.NewLine);
            textBoxResult.AppendText("} " + textBoxParStructName.Text + "_t;" + Environment.NewLine);
            textBoxResult.AppendText(Environment.NewLine);
            textBoxResult.AppendText("extern " +textBoxParStructName.Text + "_t " + textBoxParStructName.Text +";"+ Environment.NewLine);
            textBoxResult.AppendText(Environment.NewLine);

            textBoxResult.AppendText(" #define\tMENUE_PARCOUNT\t" + parcount + "\t// number of parameters" + Environment.NewLine);
			textBoxResult.AppendText(Environment.NewLine);

            textBoxResult.AppendText("// Action Prototypes" + Environment.NewLine);
            ProcessActionPrototypes(treeView1.Nodes[0]);
            textBoxResult.AppendText(Environment.NewLine);


            // Process the text definitions
            textBoxResult.AppendText("// Text definitions" + Environment.NewLine);
            textBoxResult.AppendText("#ifndef MENUE_TEXT_VARDEF" + Environment.NewLine);            
            textBoxResult.AppendText("#define MENUE_TEXT_VARDEF \\" + Environment.NewLine);
            textBoxResult.AppendText("" + ProcessNode(tbdef, tn) +" \\" + Environment.NewLine);
            m_NodeCounterForID = 0; // fixme rename
            tNodeTagInfo nt;
            nt = (tNodeTagInfo)tn.Tag;
            nt.ID = m_NodeCounterForID++;
            tn.Tag = nt;
            processTextDefinitions(tn);
            textBoxResult.AppendText(Environment.NewLine);
            textBoxResult.AppendText("#endif" + Environment.NewLine);

        
            // Parameter list
            textBoxResult.AppendText(Environment.NewLine);
            textBoxResult.AppendText("// Parameter definitions" + Environment.NewLine);
            textBoxResult.AppendText("#ifndef MENUE_PARAM_VARDEF" + Environment.NewLine); 
            textBoxResult.AppendText("#define MENUE_PARAM_VARDEF \\" + Environment.NewLine);
            textBoxResult.AppendText(textBoxParStructName.Text + "_t " + textBoxParStructName.Text + " = { \\" + Environment.NewLine);
            ProcessParameters(treeView1.Nodes[0]);
            textBoxResult.AppendText("/*CRC*/\t0 \\" + Environment.NewLine);
            textBoxResult.AppendText("};" + Environment.NewLine);
            textBoxResult.AppendText("#endif" + Environment.NewLine);
            textBoxResult.AppendText(Environment.NewLine);


            // Process the MENUE LIST
            textBoxResult.AppendText("\t\t\t//Name\tAct\tPar\tJmp\tParent\tMemory" + Environment.NewLine);
            textBoxResult.AppendText("#ifndef MENUE_MENUE_VARDEF" + Environment.NewLine);
            textBoxResult.AppendText("#define MENUE_MENUE_VARDEF \\" + Environment.NewLine);
            textBoxResult.AppendText("MenuItem_t m_items[MENUESIZE] = { \\" + Environment.NewLine);
            //            textBoxResult.AppendText("MenuItem_t m_items[MENUESIZE] = {\\" + Environment.NewLine);	
            textBoxResult.AppendText("\t" + ProcessNode(tbmen, tn) + " \\" + Environment.NewLine);
            processMenuList(tn);
            textBoxResult.AppendText("};" + Environment.NewLine);
            textBoxResult.AppendText("#endif" + Environment.NewLine);
            textBoxResult.AppendText(Environment.NewLine);

            textBoxResult.AppendText("//******** END OF AUTO-GENERATED HEADER DO NOT EDIT!!! *********" + Environment.NewLine + Environment.NewLine);


            textBoxResult2.AppendText("//******** INSERT INTO C FILE *********" + Environment.NewLine);
            textBoxResult2.AppendText("// Text definitions" + Environment.NewLine);
            textBoxResult2.AppendText("\tMENUE_TEXT_VARDEF" + Environment.NewLine);
            textBoxResult2.AppendText("// Parameter definitions" + Environment.NewLine);
            textBoxResult2.AppendText("\tMENUE_PARAM_VARDEF" + Environment.NewLine);
            textBoxResult2.AppendText("// Menue definitions" + Environment.NewLine);
            textBoxResult2.AppendText("\tMENUE_MENUE_VARDEF" + Environment.NewLine);
            textBoxResult2.AppendText(Environment.NewLine +"//******** INSERT INTO C FILE *********" + Environment.NewLine);

        }

        void ProcessParameters(TreeNode tn)
        {
            if (((tNodeTagInfo)tn.Tag).type == eMenueElementType.parameter)
            {
                textBoxResult.AppendText("/*" + ((tNodeTagInfo)tn.Tag).info + "*/ {");
                textBoxResult.AppendText("\t0, " + ((tNodeTagInfo)tn.Tag).ParLower + ", " + ((tNodeTagInfo)tn.Tag).ParUpper + ", " + ((tNodeTagInfo)tn.Tag).ParStepSize + ", " + ((tNodeTagInfo)tn.Tag).ParType);
                textBoxResult.AppendText("}, \\" + Environment.NewLine);
            }
            foreach (TreeNode tn2 in tn.Nodes)
            {
                ProcessParameters(tn2);
            }
        }

        int parcount;

        void ProcessParameterExternals(TreeNode tn)
        {
            
            if (((tNodeTagInfo)tn.Tag).type == eMenueElementType.parameter)
            {
                textBoxResult.AppendText("      Parameter_t " + ((tNodeTagInfo)tn.Tag).info + ";" + Environment.NewLine);
                parcount++;
            }
            foreach (TreeNode tn2 in tn.Nodes)
            {
                ProcessParameterExternals(tn2);
            }
        }

        void ProcessActionPrototypes(TreeNode tn)
        {
            if (((tNodeTagInfo)tn.Tag).type == eMenueElementType.action)
            {
                textBoxResult.AppendText("void " + ((tNodeTagInfo)tn.Tag).info + " (void);" + Environment.NewLine);
            }
            foreach (TreeNode tn2 in tn.Nodes)
            {
                ProcessActionPrototypes(tn2);
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
                if (!comboBoxParType.Items.Contains(((tNodeTagInfo)tn2.Tag).ParType) && ((tNodeTagInfo)tn2.Tag).ParType != "")
                    comboBoxParType.Items.Add(((tNodeTagInfo)tn2.Tag).ParType);

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
                if (m_alRememberNodeNames.Contains(tn.Text))
                {
                    // already present
                }
                else
                {
                    // new
                    m_alRememberNodeNames.Add(tn.Text);


                    textBoxResult.AppendText("" + ProcessNode(tbdef, tn) + " \\" + Environment.NewLine);
                }
                    tNodeTagInfo tag;
                    tag = (tNodeTagInfo)tn.Tag;
                    tag.ID = m_NodeCounterForID++;
                    tn.Tag = tag;           
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
                textBoxResult.AppendText("\t" +ProcessNode(tbmen, tn) + " \\" + Environment.NewLine);
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
                                sb.Append(((tNodeTagInfo)tn.Tag).ID);
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
                                    sb.Append(((tNodeTagInfo)tn.Parent.Tag).ID);
                                break;
                            case 'A':
                                if (((tNodeTagInfo)tn.Tag).type == eMenueElementType.action)
                                    sb.Append(((tNodeTagInfo)tn.Tag).info);
                                else
                                    sb.Append("0");
                                break;
                            case 'P':
                                if (((tNodeTagInfo)tn.Tag).type == eMenueElementType.parameter)
                                    sb.Append("&" + textBoxParStructName.Text + "." + ((tNodeTagInfo)tn.Tag).info);
                                else
                                    sb.Append("0");
                                break;
                            case 'j':
                                tNodeTagInfo nti;
                                if (tn.Nodes.Count > 0)
                                {
                                    // sub-nodes
                                    nti = (tNodeTagInfo)tn.Nodes[0].Tag;
                                    sb.Append(nti.ID);
                                }
                                else if (((tNodeTagInfo)tn.Tag).type == eMenueElementType.main)
                                {
                                    // main menu (first one inside menu)
                                    sb.Append("1");
                                }
                                else if (((tNodeTagInfo)tn.Tag).type == eMenueElementType.back)
                                {
                                    // back: we need the first node in the nodes collection, where our parent is. 
                                    if (tn.Parent == null)
                                        sb.Append(" **ERROR_NO_PARENT** ");
                                    else
                                    {
                                        nti = (tNodeTagInfo)tn.Parent./*FirstNode.*/Tag;
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
            textBoxInfo.Text = ((tNodeTagInfo)(e.Node.Tag)).info;
            comboBox1.SelectedIndex = (int)((tNodeTagInfo)(e.Node.Tag)).type;
            comboBoxParType.Text = ((tNodeTagInfo)(e.Node.Tag)).ParType;
            textBoxParValUpper.Text = ((tNodeTagInfo)(e.Node.Tag)).ParUpper.ToString();
            textBoxParValLower.Text = ((tNodeTagInfo)(e.Node.Tag)).ParLower.ToString();
            
            textBoxParStepSize.Text = ((tNodeTagInfo)(e.Node.Tag)).ParStepSize.ToString();
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
            tNodeTagInfo nti;
            if (tn.Tag == null)
            {
                nti = new tNodeTagInfo();
                nti.ID = -1;
                nti.info = "";
                nti.ParLower = 0;
                nti.ParUpper = 0;
                nti.ParType = "";
                nti.ParStepSize = 0;
                nti.type = eMenueElementType.normal;
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
            tNodeTagInfo nti;

            if (tn != null)
            {
                CheckTagPresenceOfNode(tn);
                tn.Text = textBoxNodeName.Text;
                nti = (tNodeTagInfo)(tn.Tag);
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
                    nti.ParStepSize = Convert.ToInt32(textBoxParStepSize.Text);
                }
                catch (Exception)
                {
                    nti.ParStepSize = 0;
                }
                nti.ParType = comboBoxParType.Text;

                nti.type = (eMenueElementType)comboBox1.SelectedIndex;
                tn.Tag = nti;

                tn.ImageIndex = (int)((tNodeTagInfo)tn.Tag).type;
                tn.SelectedImageIndex = (int)((tNodeTagInfo)tn.Tag).type;
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
                textBoxParValLower.Enabled = true;
                textBoxParValUpper.Enabled = true;
                textBoxParStepSize.Enabled = true;
                comboBoxParType.Enabled = true;
            }
            else
            {
                textBoxParValLower.Enabled = false;
                textBoxParValUpper.Enabled = false;
                textBoxParStepSize.Enabled = false;
                comboBoxParType.Enabled = false;
            }
        }

        private void treeView1_ItemDrag(object sender, ItemDragEventArgs e)
        {
            m_DragDropSourceNode = (TreeNode)e.Item;
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

        private void textBoxResult_Click(object sender, EventArgs e)
        {
            // copy all to clipboard
            Clipboard.SetData(DataFormats.Text, textBoxResult.Text);
        }

        private void textBoxResult2_Click(object sender, EventArgs e)
        {
            // copy all to clipboard
            Clipboard.SetData(DataFormats.Text, textBoxResult2.Text);
        }

     


     














    }
}

