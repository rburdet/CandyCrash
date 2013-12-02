#ifndef __USUARIOS_LIST_H__
#define __USUARIOS_LIST_H__

#include <gtkmm.h>
#include <string>

class ModelUsuarios : public Gtk::TreeModel::ColumnRecord
{
	public:
		ModelUsuarios()
		{ add(m_col_id); add(m_col_nivel); add(m_col_nombre); add(m_col_puntos);}
		Gtk::TreeModelColumn<long> m_col_id;
		Gtk::TreeModelColumn<int> m_col_nivel;
		Gtk::TreeModelColumn<std::string> m_col_nombre;
		Gtk::TreeModelColumn<int> m_col_puntos;
};

class ListaUsuarios : public Gtk::TreeView {
	public:
		ListaUsuarios(){
			m_refTreeModel = Gtk::ListStore::create(m_Columns);
			this->set_model(m_refTreeModel);

			this->append_column("Nombre", m_Columns.m_col_nombre);
			this->append_column("Nivel", m_Columns.m_col_nivel);
			this->append_column("Puntos", m_Columns.m_col_puntos);
		}
		~ListaUsuarios(){}

		void addRow(long m_col_id, std::string& m_col_nombre, int m_col_nivel,
				int m_col_puntos){
			Gtk::TreeModel::Row row = *(m_refTreeModel->append());
			row[m_Columns.m_col_id] = m_col_id;
			row[m_Columns.m_col_nombre] = m_col_nombre;
			row[m_Columns.m_col_nivel] = m_col_nivel;
			row[m_Columns.m_col_puntos] = m_col_puntos;
		}

		void getSelected(long &id, std::string & nombre, int & nivel, int &puntos){
			Glib::RefPtr<Gtk::TreeSelection> select = this->get_selection();
			Gtk::TreeModel::iterator store_iter = select->get_selected();
			Gtk::TreeModel::Row row = *store_iter;
			id = row[m_Columns.m_col_id];
			nombre = row[m_Columns.m_col_nombre];
			nivel = row[m_Columns.m_col_nivel];
			puntos = row[m_Columns.m_col_puntos];
		}

		void clearRows(){
			m_refTreeModel->clear();
		}

	protected:
		ModelUsuarios m_Columns;
		Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
};

#endif
