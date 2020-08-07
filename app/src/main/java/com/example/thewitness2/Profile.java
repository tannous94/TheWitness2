package com.example.thewitness2;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Dialog;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.firestore.DocumentReference;
import com.google.firebase.firestore.DocumentSnapshot;
import com.google.firebase.firestore.EventListener;
import com.google.firebase.firestore.FirebaseFirestore;
import com.google.firebase.firestore.FirebaseFirestoreException;
import com.google.firebase.firestore.QueryDocumentSnapshot;

import org.w3c.dom.Text;

public class Profile extends AppCompatActivity {

    private FirebaseFirestore db;

    private TextView username, player, rank, points, level, time;
    private TextView logout, reset, ranking;

    Dialog btn_reset;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_profile);

        btn_reset = new Dialog(this);

        username = (TextView) findViewById(R.id.username);
        player = (TextView) findViewById(R.id.player2);
        level = (TextView) findViewById(R.id.level2);
        rank = (TextView) findViewById(R.id.rank2);
        points = (TextView) findViewById(R.id.points2);
        time = (TextView) findViewById(R.id.time2);

        db = FirebaseFirestore.getInstance();
        logout = (TextView) findViewById(R.id.logout);
        reset = (TextView) findViewById(R.id.reset);
        ranking = (TextView) findViewById(R.id.ranks);


                Intent prev_data = getIntent();
                String doc = prev_data.getStringExtra("EmailAddr");
                DocumentReference docRef = db.collection("Accounts").document(doc);
                docRef.get().addOnCompleteListener(new OnCompleteListener<DocumentSnapshot>() {
                    @Override
                    public void onComplete(@NonNull Task<DocumentSnapshot> task) {
                        if (task.isSuccessful()) {
                            DocumentSnapshot document = task.getResult();
                            if (document.exists()) {
                                String player1 = document.getString("Player Name");
                                String level1 = document.getString("Level");
                                String rank1 = document.getString("Rank");
                                String points1 = document.getString("Points");
                                String time1 = document.getString("Time Elapsed");

                                Intent email_data = getIntent();
                                String email = email_data.getStringExtra("EmailAddr");
                                username.setText(email);
                                player.setText(player1);
                                level.setText(level1);
                                rank.setText(rank1);
                                points.setText(points1);
                                time.setText(time1);

                            } else {
                                Toast.makeText(Profile.this, "Data not found !", Toast.LENGTH_SHORT).show();
                            }
                        }
                    }
                });

        logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                logOut();
            }
        });

        reset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                resetGame(v);
            }
        });

        ranking.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String email = username.getText().toString().trim();
                Intent intent = new Intent(Profile.this, Ranking.class);
                intent.putExtra("EmailAddr", email);
                startActivity(intent);
            }
        });

    }

    public void resetGame(View v) {
        Button btn_no;
        Button btn_yes;
        username = (TextView) findViewById(R.id.username);
        btn_reset.setContentView(R.layout.activity_popup);
        btn_no =(Button) btn_reset.findViewById(R.id.no);
        btn_yes =(Button) btn_reset.findViewById(R.id.yes);
        btn_no.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                btn_reset.dismiss();
            }
        });

        btn_yes.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent prev_data = getIntent();
                String doc = prev_data.getStringExtra("EmailAddr");
                DocumentReference docRef = db.collection("Accounts").document(doc);
                docRef.update("Level", 0);
                docRef.update("Points", 0);
                docRef.update("Rank", 0);
                docRef.update("Time Elapsed", "0");

                String email = doc;
                Intent next = new Intent(Profile.this, Profile.class);
                next.putExtra("EmailAddr", email);
                startActivity(next);
            }
        });

        btn_reset.getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
        btn_reset.show();
    }

    public void logOut() {
        Intent intent = new Intent(this, MainActivity.class);
        db.collection("LoggedIn").document("CurrentLogin").update("Email", "NON");
        startActivity(intent);
    }

}
