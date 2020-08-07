package com.example.thewitness2;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.firestore.DocumentSnapshot;
import com.google.firebase.firestore.EventListener;
import com.google.firebase.firestore.FirebaseFirestore;
import com.google.firebase.firestore.FirebaseFirestoreException;
import com.google.firebase.firestore.Query;
import com.google.firebase.firestore.QuerySnapshot;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;

public class Ranking extends AppCompatActivity {

    private TextView btn_profile;
    private ListView rankings;
    private ArrayList<StringBuilder> pnames = new ArrayList<>();
    FirebaseFirestore db;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ranking);

        db = FirebaseFirestore.getInstance();
        rankings = (ListView) findViewById(R.id.rank_list);

        db.collection("Accounts").orderBy("Points", Query.Direction.DESCENDING).addSnapshotListener(new EventListener<QuerySnapshot>() {
            @Override
            public void onEvent(@Nullable QuerySnapshot queryDocumentSnapshots, @Nullable FirebaseFirestoreException e) {
                pnames.clear();
                for (DocumentSnapshot document : queryDocumentSnapshots) {
                    StringBuilder record = new StringBuilder();
                    String name = document.getString("Player Name");
                    Integer points = document.getLong("Points").intValue();

                    //build pads
//                    StringBuilder pads = new StringBuilder("%-");
//                    Integer spaces = 50 - name.length();
//                    String num_of_pads = String.valueOf(spaces);
//                    pads.append(num_of_pads).append("s");

                    StringBuilder pads = new StringBuilder("%s");
                    Integer spaces = 35 - name.length();
                    for (Integer i = 0; i < spaces; i++)
                        pads.append("_");

                    String padded = String.format(pads.toString(), name);
                    record.append(padded).append(points.toString());
                    pnames.add(record);
                }

                ArrayAdapter<StringBuilder> adapter = new ArrayAdapter<StringBuilder>(getApplicationContext(), R.layout.list_text, pnames);
                adapter.notifyDataSetChanged();
                rankings.setAdapter(adapter);
            }
        });

        btn_profile = (TextView) findViewById(R.id.back3);
        btn_profile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent prev_data = getIntent();
                String doc = prev_data.getStringExtra("EmailAddr");
                Intent intent = new Intent(Ranking.this, Profile.class);
                intent.putExtra("EmailAddr", doc);
                startActivity(intent);
            }
        });
    }
}
