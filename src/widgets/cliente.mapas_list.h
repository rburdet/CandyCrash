#ifndef __MAPAS_LIST_H__
#define __MAPAS_LIST_H__

#include <gtkmm.h>
#include <string>

class MapasColumns : public Gtk::TreeModel::ColumnRecord
{
	public:

		MapasColumns()
		{ add(m_col_nivel); add(m_col_nombre);}

		Gtk::TreeModelColumn<int> m_col_nivel;
		Gtk::TreeModelColumn<std::string> m_col_nombre;
};

class ListaMapas : public Gtk::TreeView {
	public:
		ListaMapas(){
			m_refTreeModel = Gtk::ListStore::create(m_Columns);
			this->set_model(m_refTreeModel);

			//This number will be shown with the default numeric formatting.
			this->append_column("Nivel", m_Columns.m_col_nivel);
			this->append_column("Nombre", m_Columns.m_col_nombre);

			//Make all the columns reorderable:
			//This is not necessary, but it's nice to show the feature.
			//You can use TreeView::set_column_drag_function() to more
			//finely control column drag and drop.
			for(guint i = 0; i < 2; i++)
			{
				Gtk::TreeView::Column* pColumn = this->get_column(i);
				pColumn->set_reorderable();
			}
		}
		~ListaMapas(){};

		void addRow(int m_col_nivel, std::string m_col_nombre){
			Gtk::TreeModel::Row row = *(m_refTreeModel->append());
			row[m_Columns.m_col_nivel] = m_col_nivel;
			row[m_Columns.m_col_nombre] = m_col_nombre;
		}

		void getSelected(int & nivel, std::string& name){
			Glib::RefPtr<Gtk::TreeSelection> select = this->get_selection();
			Gtk::TreeModel::iterator store_iter = select->get_selected();
			Gtk::TreeModel::Row row = *store_iter;
			nivel = row[m_Columns.m_col_nivel];
			name = row[m_Columns.m_col_nombre];
		}

		void clearRows(){
			m_refTreeModel->clear();
		}

	protected:
		MapasColumns m_Columns;
		Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

};
#endif
