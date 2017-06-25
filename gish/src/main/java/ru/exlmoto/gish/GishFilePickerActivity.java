/************************************************************************************
** The MIT License (MIT)
**
** Copyright (c) 2017 EXL
** Copyright (c) 2016 Solexid
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
************************************************************************************/

package ru.exlmoto.gish;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

public class GishFilePickerActivity extends Activity {

    private static File currentPath = null;
    private FArrayAdapter adapter = null;
    private static ListView delta = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.gish_filepicker);

        if (currentPath == null) {
            String startPath = Environment.getExternalStorageDirectory().toString();
            currentPath = new File(startPath);
        }

        Button buttonOk = (Button) findViewById(R.id.buttonOk);
        buttonOk.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View view) {
                onOkButtonClick(view);
            }
        });

        Button buttonCancel = (Button) findViewById(R.id.buttonCancel);
        buttonCancel.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View view) {
                onCancelButtonClick(view);
            }
        });

        fillFileList(currentPath);
    }

    public void onOkButtonClick(View v) {
        Intent intent = new Intent();
        intent.putExtra("GishPath", currentPath.toString() + "/");
        setResult(RESULT_OK, intent);
        finish();
    }

    public void onCancelButtonClick(View v) {
        Intent intent = new Intent();
        setResult(RESULT_CANCELED, intent);
        finish();
    }

    private void fillFileList(File startPath) {
        new FillListTask(startPath).execute();
    }

    private class FillListTask extends AsyncTask<Void, Void, List<FItem>> {

        File s_startPath = null;

        public FillListTask(File a_startFolder) {
            s_startPath = a_startFolder;
        }

        @Override
        protected List<FItem> doInBackground(Void... params) {
            File[] listFiles = s_startPath.listFiles();
            List<FItem> listItemsDirs = new ArrayList<FItem>();
            List<FItem> listItemsFiles = new ArrayList<FItem>();

            while (listFiles == null) {
                String parent = s_startPath.getParent();
                if (parent != null) {
                    s_startPath = new File(parent);
                } else {
                    s_startPath = new File(Environment.getExternalStorageDirectory().toString());
                }
                listFiles = s_startPath.listFiles();
            }

            for (File file : listFiles) {
                Date lastModifiedDate = new Date(file.lastModified());
                SimpleDateFormat formatter = new SimpleDateFormat("dd-MMM-yyyy, HH:mm:ss", Locale.getDefault());
                String dateModified = formatter.format(lastModifiedDate);
                if (file.isDirectory()) {
                    File[] dirFiles = file.listFiles();
                    int num;
                    if (dirFiles != null) {
                        num = dirFiles.length;
                    } else {
                        num = 0;
                    }
                    String numItems = String.valueOf(num);
                    if (num == 0) {
                        numItems = getResources().getString(R.string.no_items);
                    } else if (num == 1) {
                        numItems += " " + getResources().getString(R.string.item);
                    } else {
                        numItems += " " + getResources().getString(R.string.items);
                    }
                    listItemsDirs.add(new FItem(file.getName(), numItems, dateModified,
                            file.getAbsolutePath(),
                            (file.getName().toLowerCase().contains("gish") &&
                                    (!file.getName().equals(".gish")) &&
                                    (!file.getName().equals("Gish.app"))) ?
                                    R.drawable.folder_gish : R.drawable.folder));
                }
            }

            Collections.sort(listItemsDirs);
            Collections.sort(listItemsFiles);
            listItemsDirs.addAll(listItemsFiles);
            if(s_startPath.getPath().length() > 1) {
                listItemsDirs.add(0, new FItem("..", getResources().getString(R.string.parent_dir), "",
                        s_startPath.getParent(), R.drawable.folder_up));
            }
            return listItemsDirs;
        }

        @Override
        protected void onPostExecute(List<FItem> result) {
            adapter = new FArrayAdapter(GishFilePickerActivity.this, R.layout.gish_filepicker_row, result);
            delta = (ListView) findViewById(R.id.FileView);
            delta.setAdapter(adapter);
            delta.setOnItemClickListener(new OnItemClickListener() {

                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    FItem fItem = adapter.getItem(position);
                    currentPath = new File(fItem.getPath());
                    fillFileList(currentPath);
                }
            });
        }
    }
}

//--- FArrayAdapter
class FArrayAdapter extends ArrayAdapter<FItem> {

    private Context context;
    private int textViewResourceId;
    private List<FItem> itemsList;

    public FArrayAdapter(Context a_context, int a_textViewResourceId,
            List<FItem> a_itmesList) {
        super(a_context, a_textViewResourceId, a_itmesList);
        context = a_context;
        textViewResourceId = a_textViewResourceId;
        itemsList = a_itmesList;
    }

    public FItem getFItem(int i) {
        return itemsList.get(i);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        View view = convertView;
        if (view == null) {
            LayoutInflater layoutInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            view = layoutInflater.inflate(textViewResourceId, null);
        }

        final FItem fInstance = itemsList.get(position);
        if (fInstance != null) {
            TextView fileName = (TextView) view.findViewById(R.id.fileName);
            TextView fileItems = (TextView) view.findViewById(R.id.fileItems);
            TextView fileDate = (TextView) view.findViewById(R.id.fileDate);
            ImageView imageIcon = (ImageView) view.findViewById(R.id.fileIcon);

            @SuppressWarnings("deprecation")
            Drawable drawable = context.getResources().getDrawable(fInstance.getImageId());
            imageIcon.setImageDrawable(drawable);

            if (fileName != null) {
                fileName.setText(fInstance.getName());
            }
            if (fileItems != null) {
                fileItems.setText(fInstance.getItemsData());
            }
            if (fileDate != null) {
                fileDate.setText(fInstance.getDate());
            }
        }
        return view;
    }
}

//--- FItemList
class FItem implements Comparable<FItem> {

    private String name;
    private String data;
    private String date;
    private String path;
    private int imageId;

    public FItem(String a_name, String a_data,
            String a_date, String a_path, int a_imageId) {
        name = a_name;
        data = a_data;
        date = a_date;
        path = a_path;
        imageId = a_imageId;
    }

    public String getName() {
        return name;
    }

    public String getItemsData() {
        return data;
    }

    public String getDate() {
        return date;
    }

    public String getPath() {
        return path;
    }

    public int getImageId() {
        return imageId;
    }

    @Override
    public int compareTo(FItem another) {
        if (this.name != null) {
            return this.name.toLowerCase().compareTo(another.getName().toLowerCase());
        } else {
            throw new IllegalArgumentException();
        }
    }
}
