using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml;
using System.Runtime.Serialization.Formatters.Binary;
using System.Collections;

namespace Treebuilder
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            treeView1.SelectedNode = treeView1.Nodes[0];
            CheckTagPresenceOfSelectedNode();
        }

        // Xml tag for node, e.g. 'node' in case of <node></node>

        private const string XmlNodeTag = "node";

        // Xml attributes for node e.g. <node text="Asia" tag="" 

        // imageindex="1"></node>

        private const string XmlNodeTextAtt = "text";
        private const string XmlNodeTagTypeAtt = "tagType";
        private const string XmlNodeTagIDAtt = "tagID";
        private const string XmlNodeTagInfoAtt = "tagInfo";
        private const string XmlNodeImageIndexAtt = "imageindex";

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

            textBoxResult.Clear();

            TreeNode tn = treeView1.Nodes[0];

            textBoxResult.AppendText(ProcessNode(tbdef1.Text, tn) + Environment.NewLine);


            number = 0;

            NodeTagInfo nt;
            nt = (NodeTagInfo)tn.Tag;
            nt.ID = number++;
            tn.Tag = nt;

            processTextDefinitions(tn);
            textBoxResult.AppendText(Environment.NewLine);

            textBoxResult.AppendText(ProcessNode(tbmen1.Text, tn) + Environment.NewLine);
            processMenuList(tn);


        }

        string makename(TreeNode tn)
        {
            return textBoxtxtPrefix.Text + CleanString(tn.Text) + tn.Level.ToString() + "_" + tn.Index.ToString();
        }

        private void processTextDefinitions(TreeNode ano)
        {


            foreach (TreeNode tn in ano.Nodes)
            {
                NodeTagInfo nt;
                nt = (NodeTagInfo)tn.Tag;
                nt.ID = number++;
                tn.Tag = nt;
                textBoxResult.AppendText(ProcessNode(tbdef1.Text, tn) + Environment.NewLine);
            }
            foreach (TreeNode tn in ano.Nodes)
            {
                processTextDefinitions(tn);
            }
        }



        private void processMenuList(TreeNode ano)
        {
            foreach (TreeNode tn in ano.Nodes)
            {
                textBoxResult.AppendText(ProcessNode(tbmen1.Text, tn) + Environment.NewLine);
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
                        sb.Append(c);
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
                                    sb.Append(((NodeTagInfo)tn.Tag).info);
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
                nti.Action = textBoxAction.Text;
                treeView1.SelectedNode.Tag = nti;
            }*/
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            NodeTagInfo nti;

            if (treeView1.SelectedNode != null)
            {

                TreeNode tn = treeView1.SelectedNode.Nodes.Add(textBoxNodeName.Text);
                treeView1.SelectedNode.Expand();



                if (tn.Tag == null)
                {
                    nti = new NodeTagInfo();
                    tn.Tag = nti;
                }


                nti = (NodeTagInfo)(tn.Tag);
                if (textBoxInfo.Text == null)
                    nti.info = "";
                else
                    nti.info = textBoxInfo.Text;
                nti.type = (MenueElementType)comboBox1.SelectedIndex;
                nti.type = MenueElementType.normal;
                treeView1.SelectedNode.Tag = nti;


                textBoxNodeName.SelectAll();

                //treeView1.SelectedNode = tn;
            }

        }

        private void CheckTagPresenceOfSelectedNode()
        {
            NodeTagInfo nti;
            if (treeView1.SelectedNode.Tag == null)
            {
                nti = new NodeTagInfo();
                nti.ID = -1;
                nti.info = "";
                nti.type = MenueElementType.normal;
                treeView1.SelectedNode.Tag = nti;
            }
        }

        private void buttonRemove_Click(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode.Name != "root")
                treeView1.SelectedNode.Remove();
        }

        private void buttonRename_Click(object sender, EventArgs e)
        {
            NodeTagInfo nti;

            //rename
            if (treeView1.SelectedNode != null)
            {
                CheckTagPresenceOfSelectedNode();
                treeView1.SelectedNode.Text = textBoxNodeName.Text;
                nti = (NodeTagInfo)(treeView1.SelectedNode.Tag);
                nti.info = textBoxInfo.Text;
                nti.type = (MenueElementType)comboBox1.SelectedIndex;
                treeView1.SelectedNode.Tag = nti;
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox1.SelectedIndex == 3 || comboBox1.SelectedIndex == 4)
            {
                textBoxInfo.Visible = true;
            }
            else
            {
                textBoxInfo.Visible = false;
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


/*
 
 Menüpunkt als Parameter deklarieren
  Parameter Struct definieren, oder nur Standard? (name,vlaue,min,max,type/unit)
  und natürlich ;-) zuweisen &Ladestrom etc...

- Standard "Back" (eine Ebene hoch) oder "Home" (ganz an den Anfang) Menüpunkte anlegen
- Menüsimulator :-)
- Kommentare einfügen
- Menü - Defines
  #define MENUESIZE 			94      //nr of menu itmes (array size)
  #define ITEMS_PER_PAGE 		3	//nr of items per menu page 
  #define MAX_ITEM_NAME_CHARLENGTH 	20	//max nr of chars for item names
  #define MAX_SRAM_ITEM_NAME_LENGHT 	16	//max nr of chars for SRAM item names
 
 */